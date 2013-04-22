// Interpreter.cpp: implementation of the CInterpreter class.
//
//////////////////////////////////////////////////////////////////////

#include "Interpreter.h"
#include <iostream>
using namespace std;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
int CInterpreter::value = 1;
CInterpreter::CInterpreter()
{
 
} 

CInterpreter::~CInterpreter()
{

}
void CInterpreter::createNewSym(CLanguage &L,string &strValue)
{
    CSymbol sy(strMap[strValue],strValue);
    L.InsertSymbol(sy);
}
TokenType CInterpreter::getToken(CLanguage &L)
{
    char ch = 0;
    do 
    {//�����հף�����\n
        if (!std::cin.get(ch))return END;
        if ('$' == ch ) return END;
    }while (isspace(ch) /*&& ch != '\n'*/ );
    //���ٽ����з���ΪĬ��print
    if ( ch == ';'/*|| ch == '\n'*/) return PRINT;
    if ( isalpha(ch) )
    {
        string_value = ch;
        while (cin.get(ch)&& isalnum(ch) )
        {
            string_value += (ch);
        }
        //ȡ��һ�������ֻ���ĸ�ķ���
    	cin.putback(ch);
        if ( 0 == strMap[string_value] )
        {
            int &v = strMap[string_value];
            v = value++;
            //create a new symbol in language
            createNewSym(L,string_value);
        }
        return NAME; 
    }
    else if ( ch == '\'' ) 
    {
        string_value = "";
        while ( cin.get(ch)&& ch != '\'' )
        {
            string_value += (ch);
        }
        //cin���ٻطţ�'���ű��Ե�
        if ( 0 == strMap[string_value] )
        {
            int &v = strMap[string_value];
            v = value++;
            //create a new symbol in language
            createNewSym(L,string_value);
        }
        return NAME;
    }
    else if( ch == '-' ) //�����->
    {
        string_value = ch;
        if ( cin.get(ch)&&( ch == '>' ) )
        {
            string_value += ch;
            return CONDUCTOR;
        } 
        else 
        { 
            cin.putback(ch);
            if ( 0 == strMap[string_value] )
            {
                int &v = strMap[string_value];
                v = value++;
                createNewSym(L,string_value);
            }
            return NAME;
        } 
    }//end else if (ch == '-' )
    //��дaction Block
    else if ( ch == '{' )
    {
        action_string = "";
        while ( cin.get(ch)&&( ch != '}' ) )
        {
            if (ch == '{')
            {
                cin.putback(ch);
                InterBlock();
            }
            else
            {
                action_string = action_string + ch;
            }
        }
        //cout<<"action_string:"<<action_string;
        return BLOCK;
    }
    else
    {
        string_value = ch;
        if ( 0 == strMap[string_value] )
        {
            int &v = strMap[string_value];
            v = value++;
            createNewSym(L,string_value);
        }
        return NAME;
    }
}
void CInterpreter::getRules(CLanguage &L)
{
    enum STATE 
    { LEFT, ARROW,RIGHT,ACTION} state = LEFT;
    TokenType ty;
    vector<RULTYPE> RuleSet;
    //���½��з���
    RULTYPE aRule;
    aRule.rightpart.clear();
    getDefine(L);
    while ( END != (ty = getToken(L)))
    { 
        switch( state )
        {
            case LEFT: 
                //match name, NOT match return error 
                if ( ty != NAME &&
                    ty != PRINT)
                {
                    cout<<"LEFT"<<endl;
                    error();
                    return ;
                }
               ///* if ( ty == PRINT )
               // {
               //     state = LEFT;
               // }
               // else*/
               //{
                    //get leftpart
                    aRule.leftpart = strMap[string_value];
                    //set state to ARROW
                    state = ARROW;
                //}
                break;

            case ARROW:
                //match conductor, NOT match return error
                if ( ty != CONDUCTOR )
                {
                    cout<<"CONDUCTOR"<<endl;
                    error();
                    return ;
                }
                state = RIGHT;
                break;
            case RIGHT:
                //match name, NOT match return error    
                 if ( ty != NAME && ty != PRINT && ty !=BLOCK )
                {
                    cout<<"RIGHT"<<endl;
                    error();
                    return ;
                }
                if ( ty == NAME )
                {
                    //get RightPart
                    aRule.rightpart.push_back(strMap[string_value]);
                    state = RIGHT;
                }
                else if ( ty == BLOCK || ty == PRINT)
                {
                    if ( ty == BLOCK )
                    {
                        //��action  �����ʽ��
                        this->ActionVec.push_back(this->action_string);
                        if ( PRINT ==
                            ( ty = getToken( L) )  )
                        {
                            RuleSet.push_back(aRule);
                            aRule.rightpart.clear();
                        }
                        else 
                        {
                            cout<<"ty is:"<<ty<<endl;
                            cout<<"BLOCK"<<endl;
                            error();
                        }
                    }
                    else if ( ty == PRINT )
                    {
                        //��ActionVec �����action_string 
                        ActionVec.push_back("");
                        RuleSet.push_back(aRule);
                        aRule.rightpart.clear();
                    }
                    //set state to LEFT
                    state = LEFT;
                }  
                break;

            default:
                    cout<<"default"<<endl;
                error();
                break;
        }
    }
    //����ruleset ����Language ��rules
    for ( int i = 0 ; i < RuleSet.size(); i++ )
    {
       //ȡ��aRule ����rules����L 
       createNewRul(L,RuleSet[i]);
    }
}
void CInterpreter::error()
{
     std::cout<<endl
        <<"error"<<endl; 
}
//���ɷ�����
void CInterpreter::FormAnsyTable(CIset& Iset, CLanguage &L)
{
    int i, j,k;
    StartOfReduce = Iset.Icore.size();
    int nOfSymbol = L.symbol.size();//ȡ��symbol�ĸ���
    FILE *AnsyTableFile;
    vector<int> vline(nOfSymbol);
    AnsyTableFile = fopen( "AnsyTable.cpp", "w" );
    fprintf(AnsyTableFile,
        "#include <stdio.h>\n"
        "#include <string.h>\n"
        "#include <iostream>\n"
        "#define REDUCE %d\n",StartOfReduce);
    fprintf(AnsyTableFile,"/*��������Ԫ�غ���:-1��ʾ����\n"
        "n < %d Ϊshift��goto��Ŀ,nΪҪshiftջ�е�״̬����֮Ϊreduce��Ŀ\n"
        "%d ��ʾ���ܣ�����n - %dΪ��Լ��rule����*/\n",
        StartOfReduce,StartOfReduce,StartOfReduce);
    fprintf(AnsyTableFile,"#define ERROR -1\n");
    fprintf(AnsyTableFile,"#define DOLLAR 0\n");
    for ( i = 1; i < L.symbol.size(); i++)
    {
        fprintf(AnsyTableFile,"#define %s %d\n",
            L.symbol[i].name.c_str(),i );
    }
    fprintf(AnsyTableFile, "int AnsyTable[%d][%d] = {\n",
        StartOfReduce,nOfSymbol);
    fprintf(AnsyTableFile,"/*symbol  \t\t");
    for ( i = 0 ; i < nOfSymbol; i++)
    {
        fprintf(AnsyTableFile,"%5s ",L.symbol[i].name.c_str());
    }
    fprintf(AnsyTableFile,"*/");
    for (  i =0 ;i < Iset.Icore.size(); i++)
    {
        //set - 1 in line
        initLine(vline);

        fprintf(AnsyTableFile, "\n/*state %3d*/", i);
        //���Icore[i].goVec,����shift ��goto
        for (  j = 0; j < Iset.Icore[i].goVec.size(); j++ ) 
        {
               // find the index of first(symbol) 
               if ( -1 != 
                    (k = 
                        fetchIndSym(Iset.Icore[i].goVec[j].first,L)
                     )
                   )
               {
                   vline[k] = Iset.Icore[i].goVec[j].second;
               }

        }
        //��� reduce
        for ( k = 0; k < Iset.Icore[i].coreItem.size(); k++)
        {
           //���ÿ��coreItem[k],�۲��Ƿ��ܹ�reduce������ܣ��������vline��
           //Ҫʹ�ò����� L��vline,coreItem[k]
           checkReduce(L,vline,Iset.Icore[i].coreItem[k]);
        }
        //д���ļ���
        fprintf(AnsyTableFile,"{");
        for ( k = 0 ; k < nOfSymbol; k++)
        {
            fprintf(AnsyTableFile,"%5d,",vline[k]);
        }
        fprintf(AnsyTableFile,"},");
        
    }
    fprintf(AnsyTableFile, "};\n");
    fprintf(AnsyTableFile,"typedef int YYTYPE;//Need you to modify \n"
    "typedef struct \n" 
    "{\n"
    "    int state;\n"
    "    int symbol;\n"
    "} LR0TYPE;\n"
    "/*LENGTH_OF_STACK�����޸�\n*/"
    "#define LENGTH_OF_STACK 100\n"    
    "#define TOP_STATE StateStack[sp].state\n"
    "#define IS_SHIFT(act) ((act>-1 && act < REDUCE)? 1:0)\n"    
    "#define IS_REDUCE(act) ((act > REDUCE)? 1:0)\n"
    "#define IS_ACC(act)    (act == REDUCE ? 1:0)\n"
    "#define IS_ERROR(act)  (act == -1 ? 1:0)\n"    
    "#define DD  CurVal\n"
    "#define D(x) yyvalStack[sp + x]\n"
    "LR0TYPE StateStack[ LENGTH_OF_STACK ];\n"
    "YYTYPE  yyvalStack[ LENGTH_OF_STACK ];\n"
    "int sp = 0;\n\n"
    "void yy_accept()\n"
    "{\n\n"    
    "}\n"
    "void yy_error()\n"
    "{\n"
    "}\n"
    "void yy_shift(int Act,int symbol,YYTYPE val)\n"
    "{\n"
    "    sp++;\n"
    "    StateStack[sp].state  = Act;\n"
    "    StateStack[sp].symbol = symbol;\n"
    "    yyvalStack[sp] = val;\n"
    "#ifdef DEBUG\n"
    "    std::cout<<\"shift in statestake:\"<<Act<<\" shift in symbolStack\"<<symbol<<std::endl;\n"
    "#endif\n"
    "}\n\n"
    "void yy_reduce(int act,int symbol,YYTYPE val,YYTYPE *Result)\n"
    "{\n"
    "    int NA;\n"
    "    YYTYPE CurVal;\n"
    "    switch (act)\n"
    "    {\n");
    //���ɸ���case,д��ansyTableFile
    //��Ҫ֪��rules��actionstring
    FormCase(L,AnsyTableFile);
    fprintf(AnsyTableFile,"    }\n"
        "    *Result = CurVal;\n"
        "}\n");
    fprintf(AnsyTableFile,
        "void parse(int input,YYTYPE *yyval,YYTYPE *Result)\n"
        "{\n"                                               
        "    int CanUse = 1;/*check input can shift*/\n"    
        "    int Action = -1;\n"                            
        "    /*put 0 in statestack, and 0 in yyval*/\n"     
        "    memset(StateStack,0x00,sizeof(LR0TYPE));      \n"
        "    memset(yyvalStack,0x00,sizeof(YYTYPE));       \n"
        "    do                                            \n"
        "    {                                             \n"
        "        Action = AnsyTable[TOP_STATE][input];     \n"
        "        if (IS_SHIFT(Action))                     \n"
        "        {                                         \n"
        "            /*PUSH state and symbol and yyval*/   \n"
        "            yy_shift(Action,input,*yyval);        \n"
        "            CanUse = 0; /*input can not use*/     \n"
        "            /*no return*/                         \n"
        "        }                                         \n"
        "        if (IS_REDUCE(Action))                    \n"
        "        {                                         \n"
        "            yy_reduce(Action,input,*yyval,Result);\n"
        "            CanUse = 1;                           \n"
        "            //no_return                           \n"
        "        }                                         \n"
        "        if (IS_ERROR(Action))                     \n"
        "        {                                         \n"
        "            yy_error();                           \n"
        "            return;                               \n"
        "        }                                         \n"
        "        if (IS_ACC(Action))                       \n"
        "        {                                         \n"
        "            yy_accept();                          \n"
        "            return;                               \n"
        "        }                                         \n"
        "    } while (CanUse);                             \n"
        "\n"
        "}\n"
        "/*main �����������*/\n");
    fclose( AnsyTableFile );
}

/*   C   I N T E R P R E T E R . C R E A T E   N E W   R U L   */
/*-----------------------------------------------------------
    Owner: keyuchang
 Copy right belong to keyuchang
 Date 2008-3-19
    �������Ҳ���
--------------------------------------------------------------*/
void CInterpreter::createNewRul(CLanguage & L, RULTYPE & aRule)
{
#define vlang(x) (&L.symbol[x])
    
    vector<CSymbol*> rpart;
    for ( int i = 0; i < aRule.rightpart.size(); i++ )
    {
        rpart.push_back(
            vlang(aRule.rightpart[i])
            );
    }
    CRules r(
        vlang(aRule.leftpart),
        rpart);
    L.InsertRules(r);
#undef vlang 
}
//set - 1 in line
void CInterpreter::initLine(vector<int> &line)
{
    vector<int>::iterator i ;
    for ( i = line.begin(); i != line.end(); i++ )
    {
        *i = -1;
    }
    return ;
}
//���symbol ���ڵ�index
//  [2008-5-13 kyc]
int
CInterpreter::fetchIndSym(CSymbol *sy,CLanguage &L)
{
    int i;
    for ( i = 0; i < L.symbol.size(); i++ )
    {
        if (sy == &(L.symbol[i]))
        return i;
    }
    return -1;
}
//���ÿ��coreItem[k],�۲��Ƿ��ܹ�reduce������ܣ��������vline��
//Ҫʹ�ò����� L��vline
inline void CInterpreter::checkReduce( CLanguage &L,vector<int> &line,CItem &Item )
{
    //reduce ���жϷ���--��� Dot == ��RuleInd��Rule�ĸ��������Item
    //��reduce��������precs�ķ��žͰ�RuleInd��rules��Լ
#define RULELENTH(Ind) (L.rules[Ind].RightPart.size())
#define SETREDUCE(sym) line[sym] = StartOfReduce + Item.RuleInd
#define PRESET Item.precs.set 
    if (Item.Dot == RULELENTH(Item.RuleInd))
    {
        for (SET_TYPE::iterator i = PRESET.begin(); 
            i != PRESET.end(); i++ )
        {
            SETREDUCE(*i);
        }
    }
    
#undef RULELENTH
#undef SETREDUCE
#undef PRESET
}

void CInterpreter::FormCase(CLanguage &L, FILE *f)
{
    int i;
    for ( i = 1; i < L.rules.size();i++ )
    {
        //����һ��case
        fprintf(f, "\t\tcase %d:\n" ,StartOfReduce + i );
        fprintf(f,"\t\t{\n");
        fprintf(f,"\t\t\tNA = %d;\n",L.rules[i].LeftPart->value);
        fprintf(f,"\t\t\tsp = sp - %d;\n",L.rules[i].RightPart.size());
        fprintf(f, "\t\t\t%s",ActionVec[i].c_str() );
        fprintf(f, "\t\t\tyy_shift(AnsyTable[TOP_STATE][NA],NA,CurVal);\n" ) ;
        fprintf(f,"\t\t}\n");

        fprintf(f,"\n\t\t\tbreak;\n" );
    }
}

void CInterpreter::InterBlock()
{   
    char ch;
    cin.get(ch);
    if (ch == '{')
    {
        action_string += ch;
        while ( cin.get(ch)&&( ch != '}' ) )
        {
            if (ch == '{')
            {
                cin.putback(ch);
                InterBlock();
            }
            else
            {
                action_string = action_string + ch;
            }
        }
        action_string += ch;
        return;
    }
    return; 
}
/*----------------------------------------
���ܣ�����%XXX����
�ƻ�֧�����ԣ�%token��%left��%right��%nonassoc
 %{�� %} ,%start,%union , %type
��%% ����
Define->%X|eposilon {Define}
û�����Ĳ���
------------------------------------------*/
void CInterpreter::getDefine(CLanguage &L)
{

    char input;
    do 
    {//�����հף�����\n
        if (!std::cin.get(input))return ;
    }while (isspace(input)  );

    if (input == '%')//����%���� Ҫ�������´���
    {
        cout<<"IN % "<<endl;
    }
    else//����%��ͷ�����账�����˷���
    {
        cin.putback(input);
    }

    //�����Ƿ�����Ҫreturn
    return;
}
