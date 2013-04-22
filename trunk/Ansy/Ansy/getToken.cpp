#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define ERROR -1
#define DOLLAR 0
#define command 1
#define program 2
#define stmtSeq 3
#define NEWLINE 4
#define stmt 5
#define ifStmt 6
#define whileStmt 7
#define assignStmt 8
#define inputStmt 9
#define printStmt 10
#define INPUT 11
#define ID 12
#define PRINT 13
#define exp 14
#define IF 15
#define THEN 16
#define BEGIN 17
#define END 18
#define ELSE 19
#define WHILE 20
#define ASSIGN 21
#define simpleExp 22
#define LT 23
#define LE 24
#define EQ 25
#define GT 26
#define GE 27
#define NE 28
#define PLUS 29
#define term 30
#define MINUS 31
#define TIMES 32
#define factor 33
#define LP 34
#define RP 35
#define NUMBER 36
#ifdef DEBUG
    #define DPRINT printf
#else   
   #define DPRINT NULLPRINT
#endif
inline void NULLPRINT(const char *, ...)
{
}
class InterStmt;
typedef InterStmt* YYTYPE;//Need you to modify 
char TOKENINFO[][16] = 
{ 
"DOLLAR  ",
"command ",
"program ",
"stmtSeq ",
"NEWLINE    ",
"stmt ",
"ifStmt  ",
"whileStmt ",     
"assignStmt",      
"inputStmt ",     
"printStmt ",     
"INPUT     ",  
"ID        ",  
"PRINT     ",
"exp       ", 
"IF        ",
"THEN      ",
"BEGIN     ",       
"END       ", 
"ELSE      ",
"WHILE     ",
"ASSIGN    ",
"simpleExp ",
"LT  ",
"LE  ",  
"EQ  ",  
"GT  ",  
"GE  ",  
"NE  ",  
"PLUS    ",
"term    ",
"MINUS   ",
"TIMES   ", 
"factor  ", 
" LP      ",
" RP      ",
" NUMBER  " 
};
typedef enum
 { START, INCOMMENT,INNUM,INID,DONE }
   StateType;
#define MAXTOKENLENTH 128
#define MAXRESERVED    7
char TokenString[MAXTOKENLENTH + 1];
FILE *gFp =NULL; 
static struct  {
	char *str;
	int tok;
}reservedWords[MAXRESERVED]={
	{"if",IF},
	{"then",THEN},
	{"begin",BEGIN},
	{"end",END	},
	{"while",WHILE},
	{"input",INPUT	},
	{"print",PRINT}
};
struct {
    char idstr[128];
    int value;
}symbolTable[128];

static int initSymbolTable()
{
    int i = 0;
    for (i = 0 ;i < 128; i++)
    {
        strcpy(symbolTable[i].idstr,"");
        symbolTable[i].value = 0 ;
    }
    return 0;
}
static int insertValueInSymbolTable(char* idstr,int value)
{
    //search in table
    int i = 0;
    //find it modify it
    for (i = 0; i < 128; i++ )
    {
        if (!strcmp(idstr,symbolTable[i].idstr))
        {
            symbolTable[i].value = value;
            return 0;
        }
    }
    //not find it add it
    for (i = 0 ;i < 128;i++)
    {
        if (!strlen(symbolTable[i].idstr))
        {
            strcpy(symbolTable[i].idstr,idstr);
            symbolTable[i].value = value;
            break;
        }
    }
    return 0;
}
static int searchSymbolTable(char* idstr)
{
    int i;
    for (i = 0; i < 128; i++ )
    {
        if (!strcmp(idstr,symbolTable[i].idstr))
        {
            return symbolTable[i].value ;
        }
    }
    return 0;
}
static int reservedLookup(char *s)
{
	int i;
//    printf("Enter in reservedLookup\n");
	for (i = 0; i < MAXRESERVED; i++)
	{
		if (!strcmp(s,reservedWords[i].str))
		{
			return reservedWords[i].tok;
		}
	}
	return ID;
}
int gposInFile = 0;
int GetCharInFile(FILE *fp)
{
    char c = getc(fp);
    //出现下面的代码原因很简单，
    //那是因为Windows与Unix不兼容:
    //windows 换行是0x0d 0x0a,而Unix只有0x0a
    if('\n' == c)
    {
        gposInFile++; //如果为换行，额外加一。Unix类系统可能不用这样的。
    }
    gposInFile++;
    return c;
}
int UnGetCharInFile(int c ,FILE *fp)
{
    ungetc(c,fp);
    //出现下面的代码原因很简单，
    //那是因为Windows与Unix不兼容:
    //windows 换行是0x0d 0x0a,而Unix只有0x0a
    if('\n' == c)
    {
        gposInFile--; //如果为换行，额外减一。Unix类系统可能不用这样的。
    }
    gposInFile--;
    return c;
}


class simExpStmt;
class expStmt;
class stmtSeqInter;
class whileStmtInter;
class ifInterStmt;
class inputInterStmt;
class printInterStmt;
class AssignInterStmt;
class stmtInter;
class InterStmt
{
    public:
        InterStmt(){tokenstr = NULL;}
        ~InterStmt(){}
        virtual void Interpreter() {}
    char *tokenstr;
    protected:
    private:
};
class whileStmtInter:public InterStmt
{
public:
    whileStmtInter(){ }
    ~whileStmtInter(){}
    whileStmtInter(expStmt *vexpStmtPart, stmtSeqInter *vStmtSeq)
    {
        expStmtPart = vexpStmtPart;
        stmtSeqInterPart = vStmtSeq;
    }
    virtual void Interpreter();
    expStmt *expStmtPart;
    stmtSeqInter *stmtSeqInterPart;

};
class ifInterStmt:public InterStmt
{
public:
    ifInterStmt(){}
    ifInterStmt(expStmt* vexpPart,stmtSeqInter* vseqPart,stmtSeqInter*vElsePart )
    {
        expStmtPart = vexpPart;
        stmtSeqInterPart = vseqPart;
        ElseSeqPart = vElsePart;
    }
    ~ifInterStmt(){}
    expStmt *expStmtPart;
    stmtSeqInter *stmtSeqInterPart;
    stmtSeqInter *ElseSeqPart;//can NULL
    virtual void Interpreter(); 
protected:
private:
};
class stmtSeqInter:public InterStmt
{
public:
    stmtSeqInter(){};
    stmtSeqInter(stmtSeqInter* vstmtSeqPart,stmtInter *vstmt)
    {
        stmtSeqPart = vstmtSeqPart;
        stmtInterPart = vstmt;
    }
    ~stmtSeqInter(){}
     virtual void Interpreter();
     stmtSeqInter *stmtSeqPart;//can NULL
     stmtInter *stmtInterPart;
protected:
private:
};
class stmtInter:public InterStmt
{
public:
    enum stmtKind {ifStmtKind,whileStmtKind,inputStmtKind,printStmtKind,assignStmtKind};
    stmtInter(){
    }
    ~stmtInter(){}
        virtual void Interpreter();
        struct STMTKEY{
            stmtKind kind;
            InterStmt *InterStmtpart;
        }Key;
    stmtInter(stmtKind kind,InterStmt *pInter){
            Key.kind = kind;
            Key.InterStmtpart = pInter;
            }
        
protected:
private:
};
class inputInterStmt:public InterStmt
{
public:
    inputInterStmt(){};
    inputInterStmt(char* vidstr){
        idstr = strdup(vidstr);
    };
    ~inputInterStmt(){};
       virtual void Interpreter();
       char *idstr;
protected:

private:
};
class printInterStmt:public InterStmt
{
public:
        printInterStmt(){};
        printInterStmt(expStmt* vexpStmt){
            expStmtPart = vexpStmt;
        };
        ~printInterStmt(){};
        virtual void Interpreter();
        expStmt *expStmtPart;
protected:
private:
};
class  AssignInterStmt:public InterStmt
{
public:
    AssignInterStmt(){};
    AssignInterStmt(char *vidstr,expStmt* vexpStmtPart){
        idstr = strdup(vidstr);
        expStmtPart = vexpStmtPart;
    };
    ~AssignInterStmt(){};
    virtual void Interpreter();
    char *idstr;
    expStmt *expStmtPart;
protected:
private:
};
class expStmt :public InterStmt
{
public:
    expStmt(){};
     expStmt(simExpStmt *left,int op, simExpStmt* right)
     {
            LeftSimExpPart = left;
            operation = op;
            RightSimExpPart = right;
     };
     ~expStmt(){};
    virtual void Interpreter();
    int value;
    simExpStmt *LeftSimExpPart;//can null
    int operation ;
    simExpStmt *RightSimExpPart;
protected:
private:
};

class factorStmt:public expStmt
{
    public:
    enum factorKIND{idfactorKind, numfactorKind,expfactorKind };
    factorStmt(){};

    ~factorStmt(){};
    struct {
         factorKIND kind;
        union FactorKEY{
            char *idstr;
            class expStmt *ExpInFactor;
        }u;
    }Key;
    
    virtual void Interpreter();
    protected:
    private:
};
class termStmt :public expStmt
{
public:
    termStmt(){};
    termStmt(termStmt* vtermPart,factorStmt* vfactorPart){
        termPart = vtermPart;
        factorPart = vfactorPart;
    };

    ~termStmt(){};
    virtual void Interpreter();
    termStmt *termPart;//can NULL
    factorStmt *factorPart;
};
class simExpStmt: public expStmt
{
public:
    simExpStmt(){};
    simExpStmt(simExpStmt* vsimExpStmtPart,int op, termStmt* vtermPart)
    {
        simExpStmtPart = vsimExpStmtPart;
        operation = op;
        termPart = vtermPart;
    };

    ~simExpStmt(){};
    simExpStmt *simExpStmtPart;//can null
    int operation;
    termStmt *termPart;
    virtual void Interpreter();
};
int getToken(YYTYPE val)
{
	int currentToken;
	bool save;
	int TokenStringIndex = 0;
    memset(TokenString,0x00,MAXTOKENLENTH);
	StateType state = START;
	while (state != DONE)
	{
		char c = GetCharInFile(gFp);
		save = true;
		switch(state) {
		case START:
        {	if (isdigit(c))
			{
				state = INNUM;
			}
			else if (isalpha(c))
			{
				state = INID;
			}
			else if ((c==' ')||('\t'==c)||(c == '\n'))
			{
                save = false;  
            }
            else if (c== '{')//使用{表示注释
            {
                save = false;
                state = INCOMMENT;
            }
			else{
                state = DONE;
                switch(c) {
                case EOF:
                    {
                        save = false;
                    currentToken = DOLLAR;
                    }
                	break;
                case '<':
                    {
                        save = false;
                        if ('='==GetCharInFile(gFp))
                        {
                            currentToken = LE;
                        }
                        else
                        {
                            UnGetCharInFile(c,gFp);
                            currentToken = LT;
                        }
                    }
                  break;
                case '>':
                    {
                        save = false;
                        if ('=' == GetCharInFile(gFp))
                        {
                            currentToken =GE;
                        }
                        else{
                            UnGetCharInFile(c,gFp);
                            currentToken = GT;
                        }
                    }
                    break;
                case '=':
                    { 
                        save = false;
                        c = GetCharInFile(gFp);
                        if ('='==c)
                        {
                            currentToken = EQ;
                        }
                        else{
                            UnGetCharInFile(c,gFp);
                            currentToken = ASSIGN;
                        }
                    }
                    break;
                case '-':
                    {
                        save = false;
                        currentToken = MINUS;
                    }
                    break;
                case '+':
                    {
                        save = false;
                        currentToken = PLUS;
                    }
                    break;
                case '*':
                    {
                        save =false;
                        currentToken = TIMES;
                    }
                    break;
                case '(':
                    save = false;
                    currentToken = LP;
                    break;
                case ')':
                    save = false;
                    currentToken = RP;
                    break;
                case ';':
                    save =false;
                    currentToken = NEWLINE;
                    break;
                default:
                    break;
                }
			}
        }
		break;
        case INCOMMENT:
            {
                save = false;
                if (c == '}')
                {
                    state = START;
                }
            }
            break;
        case INNUM:
            {
                if (!isdigit(c))
                {
                    UnGetCharInFile(c,gFp);
                    save = false;
                    state = DONE;
                    currentToken = NUMBER;
                }
            }
            break;
        case INID:
            {
                if (!isalnum(c))
                {
                    UnGetCharInFile(c,gFp);
                    save =false;
                    state =DONE;
                    currentToken = ID;
                }
            }
            break;
        case DONE:
		default:
            state = DONE;
			currentToken = ERROR;
            break;
		}
		if ((save)&&(TokenStringIndex < MAXTOKENLENTH))
		{
			TokenString[TokenStringIndex++] = c;
		}
		if (state == DONE)
		{
			TokenString[TokenStringIndex]= '\0';
			if (ID == currentToken)
			{
				//查找保留字
				currentToken = reservedLookup(TokenString) ;
			}
		}

	}
    DPRINT("IN GetToken : Token is %s\n",TOKENINFO[currentToken]);
    DPRINT("In GetToken :TokenString is %s\n",TokenString);
    val->tokenstr = strdup(TokenString);
    DPRINT("val->Tokestr Is %s\n",val->tokenstr);
	return currentToken;
}
/////////////////-------------实现部分-------------------/////////////////////
void whileStmtInter::Interpreter()
{
    expStmtPart->Interpreter();
//    printf("In expStmtPart value %d\n",expStmtPart->value);
    while (expStmtPart->value)
    {
        stmtSeqInterPart->Interpreter();
        expStmtPart->Interpreter();
    }
}
void ifInterStmt::Interpreter()
{
    expStmtPart->Interpreter();
    if (NULL == ElseSeqPart)
    {
        if (!(expStmtPart->value))//exp 为 0
        {
            NULL;
        }
        else{
            stmtSeqInterPart->Interpreter();
        }
    }
    else{
             if (!(expStmtPart->value))//exp 为 0
             {
                ElseSeqPart->Interpreter();
             }
             else{
                stmtSeqInterPart->Interpreter();
        }
    }
}
void stmtSeqInter::Interpreter()
{

    if ( NULL == stmtSeqPart )
    {
       stmtInterPart->Interpreter();
    }
    else{
        stmtSeqPart->Interpreter();
        stmtInterPart->Interpreter();
    }
}
void stmtInter::Interpreter()
{
    Key.InterStmtpart->Interpreter();
}
void inputInterStmt::Interpreter()
{
    int val;
    scanf("%d",&val);
    insertValueInSymbolTable(idstr,val);
}
void printInterStmt::Interpreter()
{
    expStmtPart->Interpreter();

    printf("\n%d\n",expStmtPart->value);
}
void AssignInterStmt::Interpreter()
{
    expStmtPart->Interpreter();
    insertValueInSymbolTable(idstr,expStmtPart->value);
}
void expStmt::Interpreter()
{
    DPRINT("Enter in expStmt\n");
    if (NULL == LeftSimExpPart)
    {
        RightSimExpPart->Interpreter();
        value = RightSimExpPart->value;
    }
    else{
        LeftSimExpPart->Interpreter();
        RightSimExpPart->Interpreter();
        switch(operation) {
        case NE:
            value = (LeftSimExpPart->value != RightSimExpPart->value ? 1:0);
        	break;
        case GE:
            value = (LeftSimExpPart->value >= RightSimExpPart->value ? 1:0);
            break;
        case LE:
            value = (LeftSimExpPart->value <= RightSimExpPart->value ? 1:0);
            break;
        case LT:
            value = (LeftSimExpPart->value < RightSimExpPart->value ? 1:0);
            break;
        case GT:
            value = (LeftSimExpPart->value > RightSimExpPart->value ? 1:0);
            break;
        case EQ:
            value = (LeftSimExpPart->value == RightSimExpPart->value ? 1:0);
            break;
        default:
            printf("Error: beyond GE NE LT LE EQ\n");
            exit(1);
        }
    }
    DPRINT("value is %d\n",value);
    DPRINT("Exit in expStmt\n");

    return ;
}
void simExpStmt::Interpreter()
{
    //value get
    if (NULL == simExpStmtPart)
    {
        termPart->Interpreter();
        value = termPart->value;
    }
    else{
        simExpStmtPart->Interpreter();
        termPart->Interpreter();
        switch(operation) {
        case PLUS:
            value = (simExpStmtPart->value) + (termPart->value); 
        	break;
        case MINUS:
            value = (simExpStmtPart->value) - (termPart->value); 
            break;
        default:
            printf("operation error\n");
            exit(1);
        }
    }
}
void termStmt::Interpreter()
{
    if (NULL == termPart)
    {
        factorPart->Interpreter();
        value = (factorPart)->value;
    }
    else
    {
        termPart->Interpreter();
        factorPart->Interpreter();
        value = (termPart->value)*(factorPart->value);
    }
}
void factorStmt::Interpreter()
{
    switch(Key.kind) {
    case idfactorKind:
        value = searchSymbolTable(Key.u.idstr);
    	break;
    case numfactorKind:
        break;
    case expfactorKind:
        Key.u.ExpInFactor->Interpreter();
        value = Key.u.ExpInFactor->value;
        break;
    default:
        break;
    }
}
#if 0
int main()
{
	gFp = fopen("Sample.ba","r");
    if (gFp == NULL)
    {
        printf("cannot open the file\n");
        exit(1);
    }
    int tok; 
    while (tok != DOLLAR){
     tok = getToken();
     printf("Token is %s\n",TOKENINFO[tok]);
     printf("Token string %s\n",TokenString);
    }
    fclose(gFp);
	return 0;
}
#endif
void TestIf()
{
    printf("Enter in Test if\n");
    factorStmt *pFactor = new factorStmt;
    pFactor->Key.kind = factorStmt::factorKIND::numfactorKind;
    pFactor->value = 12;
    factorStmt *Yesfactor = new factorStmt;
    Yesfactor->Key.kind = factorStmt::factorKIND::numfactorKind;
    Yesfactor->value = 1;
    expStmt* pExpStmt = new expStmt(NULL, -1, new simExpStmt(NULL ,-1,new termStmt(NULL,pFactor)));
    ifInterStmt *ifTest = new ifInterStmt(pExpStmt,
        new stmtSeqInter(NULL,new stmtInter(
                                               stmtInter::stmtKind::printStmtKind, 
                                               new printInterStmt(new expStmt(NULL,-1, new simExpStmt(NULL ,-1,new termStmt(NULL,Yesfactor))
                                                        ))
                                           )      
        ),
        NULL);
    printf("if 12 then print 1\n");
    ifTest->Interpreter();
    printf("after if statement\n");
    printf("Exit in Test if\n");
}
 //////////////////////////////////////////////////////////////////////////
 //设置一个完整的语句 While 语句
 // 计算 1 + 2 + 3 +....+100
// A = 0;
// sum = 0;
// while A <= 100
// begin
//  sum = sum + A;
//  A = A + 1
// end
 //////////////////////////////////////////////////////////////////////////
void TestWhile()
{
    printf("Enter TestWhile in line %d\n",__LINE__);
    factorStmt *pFactor = new factorStmt;
    pFactor->Key.kind = factorStmt::factorKIND::numfactorKind ;
    pFactor->value = 0;
    factorStmt *pFactor100 = new factorStmt;
    pFactor100->Key.kind = factorStmt::factorKIND::numfactorKind ;
    pFactor100->value = 100;
    factorStmt *pFactorIDA = new factorStmt;
    pFactorIDA->Key.kind = factorStmt::factorKIND::idfactorKind;
    pFactorIDA->Key.u.idstr = strdup("A");

    factorStmt *pFactorIDSum = new factorStmt;
    pFactorIDSum->Key.kind = factorStmt::factorKIND::idfactorKind;
    pFactorIDSum->Key.u.idstr = strdup("sum");
    /*sum = sum + A*/
    stmtInter *stmSum = new stmtInter(stmtInter::stmtKind::assignStmtKind,
        new AssignInterStmt("sum",
            new expStmt(NULL , -1,new simExpStmt(new simExpStmt(/*sum*/NULL,-1,
                                                     new termStmt(NULL,pFactorIDSum)),
                                                     PLUS,new termStmt(/*A*/
                                                                 NULL,pFactorIDA )
             ))));
    /*A = A + 1*/
    factorStmt *pFactor1 = new factorStmt;
    pFactor1->Key.kind = factorStmt::factorKIND::numfactorKind ;
    pFactor1->value = 1;
    stmtInter *stmA = new stmtInter(stmtInter::stmtKind::assignStmtKind,
        new AssignInterStmt("A",
            new expStmt(NULL , -1,new simExpStmt(new simExpStmt(/*sum*/NULL,-1,
                                                     new termStmt(NULL,pFactorIDA)),
                                                     PLUS,new termStmt(/*A*/
                                                                 NULL,pFactor1 )
             ))));
    stmtSeqInter *WholeStmt = new stmtSeqInter(
        new stmtSeqInter(/*A = 0*/new stmtSeqInter(NULL,
                                    new stmtInter(stmtInter::stmtKind::assignStmtKind,/*Assign 语句*/
                                                  new AssignInterStmt(/*ID*/"A",
                                                        /*exp*/new expStmt(NULL,-1,
                                                                 new simExpStmt(NULL,-1,
                                                                      new termStmt(NULL,/*factor*/pFactor)))
                                                          )
                                                  )
                                ),
                          new stmtInter(stmtInter::stmtKind::assignStmtKind,/*Assign 语句*/
                                new AssignInterStmt(/*ID*/"sum",
                                                     /*exp*/new expStmt(NULL,-1,
                                                                 new simExpStmt(NULL,-1,
                                                                      new termStmt(NULL,/*factor*/pFactor)))

                                                  )
                               )/*sum = 0*/
            ),
        new stmtInter(/*while 语句*/stmtInter::stmtKind::whileStmtKind,
          new whileStmtInter(/*exp A <= 100*/new expStmt(new simExpStmt(NULL,-1, 
                                                                new termStmt(NULL,pFactorIDA)),
                                                        LE,new simExpStmt(NULL,-1,
                                                                    new termStmt(NULL,pFactor100))),
                             /*StmtSeq*/
                             new stmtSeqInter(new stmtSeqInter(/*sum = sum + A*/NULL,stmSum),
                             stmA)
                            )
                )
        );
    WholeStmt->Interpreter();

    printf("sum is %d\n",searchSymbolTable("sum"));
    printf("Exit TestWhile in line %d\n",__LINE__);

}
#if 0
int main()
{
    initSymbolTable();
    factorStmt *Pfactor = new factorStmt; 
    Pfactor->Key.kind = factorStmt::factorKIND::idfactorKind;
    Pfactor->Key.u.idstr = strdup("A");
    insertValueInSymbolTable("A",2);
    factorStmt *Anotherfactor = new factorStmt; 
    Anotherfactor->Key.kind = factorStmt::factorKIND::numfactorKind;
    Anotherfactor->value = 12;
    termStmt aterm(new termStmt(NULL,Pfactor),Anotherfactor);
    //termStmt aterm(Pfactor,Anotherfactor);
    aterm.Interpreter();
    printf("aterm value is %d\n",aterm.value);
    //aterm.Interpreter();
    simExpStmt *pSim = new simExpStmt(NULL,-1,&aterm);
    expStmt bexp(NULL,-1,pSim);
    printInterStmt aPrint(&bexp);
    aPrint.Interpreter();
    simExpStmt *pAnotherSim = new simExpStmt(pSim,PLUS,&aterm);
    pAnotherSim->Interpreter();
    printf("value is %d\n",pAnotherSim->value);
    expStmt aexp(pAnotherSim,LT,pSim);
    aexp.Interpreter();
    printf("aexp is %d\n",aexp.value);
    printf("Before assign A value is %d\n",searchSymbolTable("A"));
    AssignInterStmt assign("A",&aexp);
    assign.Interpreter();
    printf("After assign A value is %d\n",searchSymbolTable("A"));
    expStmt *E = Anotherfactor;
    factorStmt *m = (factorStmt*)E;
    m->Interpreter();
    printf("m value is %d\n",m->value);
    inputInterStmt Ainput("Ba");
    Ainput.Interpreter();
    printf("value Ba is %d\n",searchSymbolTable("Ba"));
    //////////////////////////////////////////////////////////////////////////
    //               if语句的测试
    //////////////////////////////////////////////////////////////////////////
    TestIf();

    //////////////////////////////////////////////////////////////////////////
    // while 语句测试
    //////////////////////////////////////////////////////////////////////////
    TestWhile();
    return 0;
}
 #endif