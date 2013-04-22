#include <stdio.h>
#include <string.h>
#include <iostream>
#define REDUCE 3
/*分析表中元素含义:-1表示出错
n < 3 为shift或goto项目,n为要shift栈中的状态，反之为reduce项目
3 表示接受，其中n - 3为规约的rule索引*/
#define END 0
#define c 1
#define e 2
#define t 3
int AnsyTable[3][4] = {
/*symbol  		    $     c     e     t */
/*state   0*/{   -1,   -1,    1,    2,},
/*state   1*/{    3,   -1,   -1,   -1,},
/*state   2*/{    4,   -1,   -1,   -1,},};
typedef int YYTYPE;//modify
typedef struct 
{
    int state;
    int symbol;
} LR0TYPE;
/*LENGTH_OF_STACK自行修改
*/#define LENGTH_OF_STACK 100
#define TOP_STATE StateStack[sp].state
#define IS_SHIFT(act) ((act>-1 && act < REDUCE)? 1:0)
#define IS_REDUCE(act) ((act > REDUCE)? 1:0)
#define IS_ACC(act)    (act == REDUCE ? 1:0)
#define IS_ERROR(act)  (act == -1 ? 1:0)
#define DD  CurVal
#define D(x) yyvalStack[sp + x]
LR0TYPE StateStack[ LENGTH_OF_STACK ];
YYTYPE  yyvalStack[ LENGTH_OF_STACK ];
int sp = 0;

void yy_accept()
{

}
void yy_error()
{
}
void yy_shift(int Act,int symbol,YYTYPE val)
{
    sp++;
    StateStack[sp].state  = Act;
    StateStack[sp].symbol = symbol;
    yyvalStack[sp] = val;
    std::cout<<"shift in statestake: "<<Act<<" shift in symbolStack  "<<TOKENINFO[symbol]<<std::endl;
}

void yy_reduce(int act,int symbol,YYTYPE val,YYTYPE *Result)
{
    int NA;
    YYTYPE CurVal;
    switch (act)
    {
		case 4:
			NA = 2;
			sp = sp - 1;
			
afa
{
af
{fa};
}n
			yy_shift(AnsyTable[TOP_STATE][NA],NA,CurVal);

			break;
    }
    *Result = CurVal;
}
void parse(int input,YYTYPE *yyval,YYTYPE *Result)
{
    int CanUse = 1;/*check input can shift*/
    int Action = -1;
    /*put 0 in statestack, and 0 in yyval*/
    memset(StateStack,0x00,sizeof(LR0TYPE));      
    memset(yyvalStack,0x00,sizeof(YYTYPE));       
    do                                            
    {                                             
        Action = AnsyTable[TOP_STATE][input];     
        if (IS_SHIFT(Action))                     
        {                                         
            /*PUSH state and symbol and yyval*/   
            yy_shift(Action,input,*yyval);        
            CanUse = 0; /*input can not use*/     
            /*no return*/                         
        }                                         
        if (IS_REDUCE(Action))                    
        {                                         
            yy_reduce(Action,input,*yyval,Result);
            CanUse = 1;                           
            //no_return                           
        }                                         
        if (IS_ERROR(Action))                     
        {                                         
            yy_error();                           
            return;                               
        }                                         
        if (IS_ACC(Action))                       
        {                                         
            yy_accept();                          
            return;                               
        }                                         
    } while (CanUse);                             

}
/*main 函数自行添加*/
