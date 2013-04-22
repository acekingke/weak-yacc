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
    {//跳过空白，除开\n
        if (!std::cin.get(ch))return END;
        if ('$' == ch ) return END;
    }while (isspace(ch) /*&& ch != '\n'*/ );
    //不再将换行符作为默认print
    if ( ch == ';'/*|| ch == '\n'*/) return PRINT;
    if ( isalpha(ch) )
    {
        string_value = ch;
        while (cin.get(ch)&& isalnum(ch) )
        {
            string_value += (ch);
        }
        //取到一个非数字或字母的符号
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
        //cin不再回放，'符号被吃掉
        if ( 0 == strMap[string_value] )
        {
            int &v = strMap[string_value];
            v = value++;
            //create a new symbol in language
            createNewSym(L,string_value);
        }
        return NAME;
    }
    else if( ch == '-' ) //如果是->
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
    //填写action Block
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
    //以下进行反复
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
                        //将action  表插入式中
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
                        //在ActionVec 填入空action_string 
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
    //根据ruleset 产生Language 的rules
    for ( int i = 0 ; i < RuleSet.size(); i++ )
    {
       //取出aRule 产生rules插入L 
       createNewRul(L,RuleSet[i]);
    }
}
void CInterpreter::error()
{
     std::cout<<endl
        <<"error"<<endl; 
}
//生成分析表
void CInterpreter::FormAnsyTable(CIset& Iset, CLanguage &L)
{
    int i, j,k;
    StartOfReduce = Iset.Icore.size();
    int nOfSymbol = L.symbol.size();//取得symbol的个数
    FILE *AnsyTableFile;
    vector<int> vline(nOfSymbol);
    AnsyTableFile = fopen( "AnsyTable.cpp", "w" );
    fprintf(AnsyTableFile,
        "#include <stdio.h>\n"
        "#include <string.h>\n"
        "#include <iostream>\n"
        "#define REDUCE %d\n",StartOfReduce);
    fprintf(AnsyTableFile,"/*分析表中元素含义:-1表示出错\n"
        "n < %d 为shift或goto项目,n为要shift栈中的状态，反之为reduce项目\n"
        "%d 表示接受，其中n - %d为规约的rule索引*/\n",
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
        //填充Icore[i].goVec,包含shift 和goto
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
        //填充 reduce
        for ( k = 0; k < Iset.Icore[i].coreItem.size(); k++)
        {
           //检查每个coreItem[k],观察是否能够reduce，如果能，就填充如vline中
           //要使用参数有 L，vline,coreItem[k]
           checkReduce(L,vline,Iset.Icore[i].coreItem[k]);
        }
        //写入文件中
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
    "/*LENGTH_OF_STACK自行修改\n*/"
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
    //生成各个case,写入ansyTableFile
    //需要知道rules，actionstring
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
        "/*main 函数自行添加*/\n");
    fclose( AnsyTableFile );
}

/*   C   I N T E R P R E T E R . C R E A T E   N E W   R U L   */
/*-----------------------------------------------------------
    Owner: keyuchang
 Copy right belong to keyuchang
 Date 2008-3-19
    创建并且插入
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
//获得symbol 所在的index
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
//检查每个coreItem[k],观察是否能够reduce，如果能，就填充如vline中
//要使用参数有 L，vline
inline void CInterpreter::checkReduce( CLanguage &L,vector<int> &line,CItem &Item )
{
    //reduce 的判断方法--如果 Dot == 以RuleInd的Rule的个数，则该Item
    //可reduce，遇到以precs的符号就按RuleInd号rules规约
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
        //生成一个case
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
功能：处理%XXX符号
计划支持特性：%token，%left，%right，%nonassoc
 %{， %} ,%start,%union , %type
以%% 结束
Define->%X|eposilon {Define}
没有做的部分
------------------------------------------*/
void CInterpreter::getDefine(CLanguage &L)
{

    char input;
    do 
    {//跳过空白，除开\n
        if (!std::cin.get(input))return ;
    }while (isspace(input)  );

    if (input == '%')//发现%符号 要进行以下处理
    {
        cout<<"IN % "<<endl;
    }
    else//不是%开头，不需处理，回退符号
    {
        cin.putback(input);
    }

    //无论是否处理，均要return
    return;
}
