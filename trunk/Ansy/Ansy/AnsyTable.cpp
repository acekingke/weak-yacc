//#define DEBUG
#include <stdio.h>
#include <string.h>
#include <iostream>
#define REDUCE 59
/*分析表中元素含义:-1表示出错
n < 59 为shift或goto项目,n为要shift栈中的状态，反之为reduce项目
59 表示接受，其中n - 59为规约的rule索引*/
#include "gettoken.cpp"
int AnsyTable[59][37] = {
/*symbol  		    $ command program stmtSeq NEWLINE  stmt ifStmt whileStmt assignStmt inputStmt printStmt INPUT    ID PRINT   exp    IF  THEN BEGIN   END  ELSE WHILE ASSIGN simpleExp    LT    LE    EQ    GT    GE    NE  PLUS  term MINUS TIMES factor    LP    RP NUMBER */
/*state   0*/{   -1,   -1,    1,    2,   -1,    3,    4,    5,    6,    7,    8,   12,   11,   13,   -1,    9,   -1,   -1,   -1,   -1,   10,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,},
/*state   1*/{   59,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,},
/*state   2*/{   60,   -1,   -1,   -1,   14,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,},
/*state   3*/{   62,   -1,   -1,   -1,   62,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   62,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,},
/*state   4*/{   63,   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,},
/*state   5*/{   64,   -1,   -1,   -1,   64,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   64,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,},
/*state   6*/{   65,   -1,   -1,   -1,   65,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   65,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,},
/*state   7*/{   66,   -1,   -1,   -1,   66,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   66,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,},
/*state   8*/{   67,   -1,   -1,   -1,   67,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   67,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,},
/*state   9*/{   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   20,   -1,   15,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   16,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   17,   -1,   -1,   18,   19,   -1,   21,},
/*state  10*/{   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   20,   -1,   22,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   16,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   17,   -1,   -1,   18,   19,   -1,   21,},
/*state  11*/{   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   23,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,},
/*state  12*/{   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   24,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,},
/*state  13*/{   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   20,   -1,   25,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   16,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   17,   -1,   -1,   18,   19,   -1,   21,},
/*state  14*/{   -1,   -1,   -1,   -1,   -1,   26,    4,    5,    6,    7,    8,   12,   11,   13,   -1,    9,   -1,   -1,   -1,   -1,   10,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,},
/*state  15*/{   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   27,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,},
/*state  16*/{   80,   -1,   -1,   -1,   80,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   80,   80,   80,   -1,   -1,   -1,   -1,   28,   29,   30,   31,   32,   33,   34,   -1,   35,   -1,   -1,   -1,   80,   -1,},
/*state  17*/{   83,   -1,   -1,   -1,   83,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   83,   83,   83,   -1,   -1,   -1,   -1,   83,   83,   83,   83,   83,   83,   83,   -1,   83,   36,   -1,   -1,   83,   -1,},
/*state  18*/{   85,   -1,   -1,   -1,   85,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   85,   85,   85,   -1,   -1,   -1,   -1,   85,   85,   85,   85,   85,   85,   85,   -1,   85,   85,   -1,   -1,   85,   -1,},
/*state  19*/{   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   20,   -1,   37,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   16,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   17,   -1,   -1,   18,   19,   -1,   21,},
/*state  20*/{   87,   -1,   -1,   -1,   87,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   87,   87,   87,   -1,   -1,   -1,   -1,   87,   87,   87,   87,   87,   87,   87,   -1,   87,   87,   -1,   -1,   87,   -1,},
/*state  21*/{   88,   -1,   -1,   -1,   88,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   88,   88,   88,   -1,   -1,   -1,   -1,   88,   88,   88,   88,   88,   88,   88,   -1,   88,   88,   -1,   -1,   88,   -1,},
/*state  22*/{   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   38,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,},
/*state  23*/{   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   20,   -1,   39,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   16,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   17,   -1,   -1,   18,   19,   -1,   21,},
/*state  24*/{   68,   -1,   -1,   -1,   68,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   68,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,},
/*state  25*/{   69,   -1,   -1,   -1,   69,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   69,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,},
/*state  26*/{   61,   -1,   -1,   -1,   61,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   61,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,},
/*state  27*/{   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   40,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,},
/*state  28*/{   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   20,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   41,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   17,   -1,   -1,   18,   19,   -1,   21,},
/*state  29*/{   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   20,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   42,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   17,   -1,   -1,   18,   19,   -1,   21,},
/*state  30*/{   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   20,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   43,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   17,   -1,   -1,   18,   19,   -1,   21,},
/*state  31*/{   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   20,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   44,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   17,   -1,   -1,   18,   19,   -1,   21,},
/*state  32*/{   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   20,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   17,   -1,   -1,   18,   19,   -1,   21,},
/*state  33*/{   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   20,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   46,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   17,   -1,   -1,   18,   19,   -1,   21,},
/*state  34*/{   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   20,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   47,   -1,   -1,   18,   19,   -1,   21,},
/*state  35*/{   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   20,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   48,   -1,   -1,   18,   19,   -1,   21,},
/*state  36*/{   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   20,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   49,   19,   -1,   21,},
/*state  37*/{   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   50,   -1,},
/*state  38*/{   -1,   -1,   -1,   51,   -1,    3,    4,    5,    6,    7,    8,   12,   11,   13,   -1,    9,   -1,   -1,   -1,   -1,   10,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,},
/*state  39*/{   73,   -1,   -1,   -1,   73,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   73,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,},
/*state  40*/{   -1,   -1,   -1,   52,   -1,    3,    4,    5,    6,    7,    8,   12,   11,   13,   -1,    9,   -1,   -1,   -1,   -1,   10,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,},
/*state  41*/{   74,   -1,   -1,   -1,   74,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   74,   74,   74,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   34,   -1,   35,   -1,   -1,   -1,   74,   -1,},
/*state  42*/{   75,   -1,   -1,   -1,   75,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   75,   75,   75,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   34,   -1,   35,   -1,   -1,   -1,   75,   -1,},
/*state  43*/{   76,   -1,   -1,   -1,   76,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   76,   76,   76,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   34,   -1,   35,   -1,   -1,   -1,   76,   -1,},
/*state  44*/{   77,   -1,   -1,   -1,   77,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   77,   77,   77,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   34,   -1,   35,   -1,   -1,   -1,   77,   -1,},
/*state  45*/{   78,   -1,   -1,   -1,   78,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   78,   78,   78,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   34,   -1,   35,   -1,   -1,   -1,   78,   -1,},
/*state  46*/{   79,   -1,   -1,   -1,   79,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   79,   79,   79,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   34,   -1,   35,   -1,   -1,   -1,   79,   -1,},
/*state  47*/{   81,   -1,   -1,   -1,   81,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   81,   81,   81,   -1,   -1,   -1,   -1,   81,   81,   81,   81,   81,   81,   81,   -1,   81,   36,   -1,   -1,   81,   -1,},
/*state  48*/{   82,   -1,   -1,   -1,   82,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   82,   82,   82,   -1,   -1,   -1,   -1,   82,   82,   82,   82,   82,   82,   82,   -1,   82,   36,   -1,   -1,   82,   -1,},
/*state  49*/{   84,   -1,   -1,   -1,   84,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   84,   84,   84,   -1,   -1,   -1,   -1,   84,   84,   84,   84,   84,   84,   84,   -1,   84,   84,   -1,   -1,   84,   -1,},
/*state  50*/{   86,   -1,   -1,   -1,   86,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   86,   86,   86,   -1,   -1,   -1,   -1,   86,   86,   86,   86,   86,   86,   86,   -1,   86,   86,   -1,   -1,   86,   -1,},
/*state  51*/{   -1,   -1,   -1,   -1,   14,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   53,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,},
/*state  52*/{   -1,   -1,   -1,   -1,   14,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   54,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,},
/*state  53*/{   72,   -1,   -1,   -1,   72,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   72,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,},
/*state  54*/{   70,   -1,   -1,   -1,   70,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   70,   55,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,},
/*state  55*/{   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   56,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,},
/*state  56*/{   -1,   -1,   -1,   57,   -1,    3,    4,    5,    6,    7,    8,   12,   11,   13,   -1,    9,   -1,   -1,   -1,   -1,   10,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,},
/*state  57*/{   -1,   -1,   -1,   -1,   14,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,},
/*state  58*/{   71,   -1,   -1,   -1,   71,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   71,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,},};
//typedef int YYTYPE;//Need you to modify 
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
#define D(x) (yyvalStack[sp + x])
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
    DPRINT("shift To: sp %d\n",sp);
#ifdef DEBUG
    std::cout<<"shift in statestake:"<<Act<<" shift in symbolStack"<<symbol<<std::endl;
#endif
}

void yy_reduce(int act,int symbol,YYTYPE val,YYTYPE *Result)
{
    int NA;
    YYTYPE CurVal;
    switch (act)
    {
		case 60:
		{
			NA = 2;
			sp = sp - 1;
			
  DPRINT("In Reduce:program->stmtSeq\n"); 
	D(1)->Interpreter();
			yy_shift(AnsyTable[TOP_STATE][NA],NA,CurVal);
		}

			break;
		case 61:
		{
			NA = 3;
			sp = sp - 3;
			
   DPRINT("In Reduce:stmtSeq->stmtSeq NEWLINE stmt \n");
	 DD = new stmtSeqInter((stmtSeqInter*)D(1),(stmtInter*)D(3));		
			yy_shift(AnsyTable[TOP_STATE][NA],NA,CurVal);
		}

			break;
		case 62:
		{
			NA = 3;
			sp = sp - 1;
			
   DPRINT("In Reduce:stmtSeq->stmt\n");
	 DD = new stmtSeqInter(NULL,(stmtInter*)D(1));
			yy_shift(AnsyTable[TOP_STATE][NA],NA,CurVal);
		}

			break;
		case 63:
		{
			NA = 5;
			sp = sp - 1;
			
     DPRINT("In Reduce:stmt->ifStmt\n");
		 DD = new stmtInter(stmtInter::stmtKind::ifStmtKind,(ifInterStmt*)D(1));
			yy_shift(AnsyTable[TOP_STATE][NA],NA,CurVal);
		}

			break;
		case 64:
		{
			NA = 5;
			sp = sp - 1;
			
     DPRINT("In Reduce:stmt->whileStmt\n");
		 DD = new stmtInter(stmtInter::stmtKind::whileStmtKind,(whileStmtInter*)D(1));
			yy_shift(AnsyTable[TOP_STATE][NA],NA,CurVal);
		}

			break;
		case 65:
		{
			NA = 5;
			sp = sp - 1;
			
     DPRINT("In Reduce:stmt->assignStmt\n");
		 DD = new stmtInter(stmtInter::stmtKind::assignStmtKind,(AssignInterStmt*)D(1));
			yy_shift(AnsyTable[TOP_STATE][NA],NA,CurVal);
		}

			break;
		case 66:
		{
			NA = 5;
			sp = sp - 1;
			
     DPRINT("In Reduce:stmt->inputStmt\n");
		 DD = new stmtInter(stmtInter::stmtKind::inputStmtKind,(inputInterStmt*)D(1));
			yy_shift(AnsyTable[TOP_STATE][NA],NA,CurVal);
		}

			break;
		case 67:
		{
			NA = 5;
			sp = sp - 1;
			
   DPRINT("In Reduce:stmt->printStmt\n");
	 DD = new stmtInter(stmtInter::stmtKind::printStmtKind,(printInterStmt*)D(1));
			yy_shift(AnsyTable[TOP_STATE][NA],NA,CurVal);
		}

			break;
		case 68:
		{
			NA = 9;
			sp = sp - 2;
			
    DPRINT("In Reduce:inputStmt->INPUT ID\n");
	  DD = new inputInterStmt(D(2)->tokenstr);
			yy_shift(AnsyTable[TOP_STATE][NA],NA,CurVal);
		}

			break;
		case 69:
		{
			NA = 10;
			sp = sp - 2;
			
   DPRINT("In Reduce:printStmt->PRINT exp\n");
	 DD = new printInterStmt((expStmt*)D(2));
			yy_shift(AnsyTable[TOP_STATE][NA],NA,CurVal);
		}

			break;
		case 70:
		{
			NA = 6;
			sp = sp - 6;
			
     DPRINT("In Reduce:ifStmt->IF exp THEN BEGIN stmtSeq END\n");
     DD = new ifInterStmt((expStmt*)D(2),(stmtSeqInter*)D(5),NULL);
			yy_shift(AnsyTable[TOP_STATE][NA],NA,CurVal);
		}

			break;
		case 71:
		{
			NA = 6;
			sp = sp - 10;
			
   DPRINT("In Reduce:ifStmt->IF exp THEN BEGIN stmtSeq END ELSE BEGIN stmtSeq END\n");
   DD = new ifInterStmt((expStmt*)D(2),(stmtSeqInter*)D(5),(stmtSeqInter*)D(9));
			yy_shift(AnsyTable[TOP_STATE][NA],NA,CurVal);
		}

			break;
		case 72:
		{
			NA = 7;
			sp = sp - 5;
			
   DPRINT("In Reduce:whileStmt->WHILE exp BEGIN stmtSeq END\n");
   DD = new whileStmtInter((expStmt*)D(2),(stmtSeqInter*)D(4));
			yy_shift(AnsyTable[TOP_STATE][NA],NA,CurVal);
		}

			break;
		case 73:
		{
			NA = 8;
			sp = sp - 3;
			
   DPRINT("In Reduce:assignStmt->ID ASSIGN exp\n");
	 DD = new AssignInterStmt(D(1)->tokenstr,(expStmt*)D(3));
			yy_shift(AnsyTable[TOP_STATE][NA],NA,CurVal);
		}

			break;
		case 74:
		{
			NA = 14;
			sp = sp - 3;
			
   DPRINT("In Reduce:exp->simpleExp LT simpleExp\n");
	 DD = new expStmt((simExpStmt*)D(1),LT,(simExpStmt*)D(3));	
			yy_shift(AnsyTable[TOP_STATE][NA],NA,CurVal);
		}

			break;
		case 75:
		{
			NA = 14;
			sp = sp - 3;
			
   DPRINT("In Reduce:exp->simpleExp LE simpleExp\n");
	 DD = new expStmt((simExpStmt*)D(1),LE,(simExpStmt*)D(3));	
			yy_shift(AnsyTable[TOP_STATE][NA],NA,CurVal);
		}

			break;
		case 76:
		{
			NA = 14;
			sp = sp - 3;
			
   DPRINT("In Reduce:exp->simpleExp EQ simpleExp\n");
	 DD = new expStmt((simExpStmt*)D(1),EQ,(simExpStmt*)D(3));
			yy_shift(AnsyTable[TOP_STATE][NA],NA,CurVal);
		}

			break;
		case 77:
		{
			NA = 14;
			sp = sp - 3;
			
   DPRINT("In Reduce:exp->simpleExp GT simpleExp\n");
	 DD = new expStmt((simExpStmt*)D(1),GT,(simExpStmt*)D(3));
			yy_shift(AnsyTable[TOP_STATE][NA],NA,CurVal);
		}

			break;
		case 78:
		{
			NA = 14;
			sp = sp - 3;
			
   DPRINT("In Reduce:exp->simpleExp GE simpleExp\n");
	 DD = new expStmt((simExpStmt*)D(1),GE,(simExpStmt*)D(3));
			yy_shift(AnsyTable[TOP_STATE][NA],NA,CurVal);
		}

			break;
		case 79:
		{
			NA = 14;
			sp = sp - 3;
			
   DPRINT("In Reduce:exp->simpleExp NE simpleExp\n");
	 DD = new expStmt((simExpStmt*)D(1),NE,(simExpStmt*)D(3));
			yy_shift(AnsyTable[TOP_STATE][NA],NA,CurVal);
		}

			break;
		case 80:
		{
			NA = 14;
			sp = sp - 1;
			
   DPRINT("In Reduce:exp->simpleExp\n");
	 DD = new expStmt(NULL,-1,(simExpStmt*)D(1));
			yy_shift(AnsyTable[TOP_STATE][NA],NA,CurVal);
		}

			break;
		case 81:
		{
			NA = 22;
			sp = sp - 3;
			
   DPRINT("In Reduce:simpleExp->simpleExp PLUS term\n");
	 DD = new simExpStmt((simExpStmt*)D(1),PLUS,(termStmt*)D(3)); 
			yy_shift(AnsyTable[TOP_STATE][NA],NA,CurVal);
		}

			break;
		case 82:
		{
			NA = 22;
			sp = sp - 3;
			
   DPRINT("In Reduce:simpleExp->simpleExp MINUS term\n");
	 DD = new simExpStmt((simExpStmt*)D(1),MINUS,(termStmt*)D(3)); 
			yy_shift(AnsyTable[TOP_STATE][NA],NA,CurVal);
		}

			break;
		case 83:
		{
			NA = 22;
			sp = sp - 1;
			
   DPRINT("In Reduce:simpleExp->term\n");
	 DD = new simExpStmt(NULL,-1,(termStmt*)D(1)); 
			yy_shift(AnsyTable[TOP_STATE][NA],NA,CurVal);
		}

			break;
		case 84:
		{
			NA = 30;
			sp = sp - 3;
			
  DPRINT("In Reduce:term->term TIMES factor\n");
	DD = new termStmt((termStmt*)D(1),(factorStmt*)D(3)); 
 			yy_shift(AnsyTable[TOP_STATE][NA],NA,CurVal);
		}

			break;
		case 85:
		{
			NA = 30;
			sp = sp - 1;
			
  DPRINT("In Reduce:term->factor\n");
	DD = new termStmt(NULL,(factorStmt*)D(1)); 
			yy_shift(AnsyTable[TOP_STATE][NA],NA,CurVal);
		}

			break;
		case 86:
		{
			NA = 33;
			sp = sp - 3;
			
  DPRINT("In Reduce:factor->LP exp RP\n");
	factorStmt *p=  new factorStmt;
	p->Key.kind = factorStmt::factorKIND::expfactorKind;
	p->Key.u.ExpInFactor = (expStmt*)D(2);
			yy_shift(AnsyTable[TOP_STATE][NA],NA,CurVal);
		}

			break;
		case 87:
		{
			NA = 33;
			sp = sp - 1;
			 
   DPRINT("In Reduce:factor->ID\n");
	factorStmt* p =  new factorStmt;
	p->Key.kind = factorStmt::factorKIND::idfactorKind;
	p->Key.u.idstr = D(1)->tokenstr;
	DD = p;
			yy_shift(AnsyTable[TOP_STATE][NA],NA,CurVal);
		}

			break;
		case 88:
		{
			NA = 33;
			sp = sp - 1;
			
   DPRINT("In Reduce:factor->NUMBER\n");
   factorStmt*p  = new factorStmt;
	p->Key.kind = factorStmt::factorKIND::numfactorKind;
	p->value = atoi(D(1)->tokenstr);
    DPRINT("DD val is %d\n",p->value);
	DD = p;
			yy_shift(AnsyTable[TOP_STATE][NA],NA,CurVal);
		}

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
            DPRINT("Before shift : input is %s\n",TOKENINFO[input]);
            DPRINT("Before shift :yyval token is %s\n",(*yyval)->tokenstr);
            DPRINT("Before shift :Sp is %d\n",sp);
            yy_shift(Action,input,*yyval);        
            CanUse = 0; /*input can not use*/     
            /*no return*/                         
        }                                         
        if (IS_REDUCE(Action))                    
        { 
            DPRINT("Before Reduce :input %s\n",TOKENINFO[input]);
            DPRINT("Before Reduce :Sp %d\n",sp);
            DPRINT("Before Reduce : Top val string %s\n",(yyvalStack[sp])->tokenstr);
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
void Ansyparse()
{
    DPRINT("Enter Ansyparse\n");
    initSymbolTable();
    YYTYPE pVal ;
    YYTYPE pRes;
	gFp = fopen("Sample.ba","r");
    if (gFp == NULL)
    {
        printf("cannot open the file\n");
        exit(1);
    }
    int tok; 
    while (tok != DOLLAR){
     pVal  = new InterStmt;
     tok = getToken(pVal);
#ifdef DEBUG
     printf("Token is %s\n",TOKENINFO[tok]);
     printf("Token string %s\n",TokenString);
#endif

     parse(tok,&pVal,&pRes);

    }
    fclose(gFp);    
    DPRINT("Exit Ansyparse\n");

}
int main()
{
    DPRINT("Now Enter the main\n");
    Ansyparse();
    return 0;
}