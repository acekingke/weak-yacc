// Interpreter.h: interface for the CInterpreter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INTERPRETER_H__4E868C25_AF96_41D5_A44F_2A29EF89B36E__INCLUDED_)
#define AFX_INTERPRETER_H__4E868C25_AF96_41D5_A44F_2A29EF89B36E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <map>
#include <string>
#include "Language.h"
#include "Iset.h"
#include <stdio.h> //需要使用fprintf
enum TokenType 
{
    NAME, CONDUCTOR,END,PRINT,
    BLOCK,
    ERROR
};
    struct RULTYPE
    {
        int leftpart;
        vector<int> rightpart;
    } ;
class CInterpreter  
{
public:
	void InterBlock();
	void FormCase(CLanguage &L,FILE* f);
    CInterpreter();
    virtual ~CInterpreter();
    TokenType getToken(CLanguage &L);
    std::string string_value;
    //取得rules 
    void getRules(CLanguage &L);
    string action_string;
    int StartOfReduce ;
    /* -1 为 出错，0 ~ StartOfReduce -1   表示shift 或 goto
    StartOfReduce ~为ruduce 
    */
    vector<std::string> ActionVec;
    std::map<std::string,int> strMap;
void FormAnsyTable(CIset& Iset, CLanguage &L);
private:
	void getDefine(CLanguage &L);
    static int value;
    void createNewSym(CLanguage &L,string &strValue);
    void initLine(vector<int> &line);
    // fetch index of symbol input ,if error return -1,else return the index  
    int  fetchIndSym(CSymbol *sy,CLanguage &L);
    void error();
    void createNewRul(CLanguage &L, RULTYPE & aRule);
    inline void checkReduce( CLanguage &L,vector<int> &line,CItem &Item );
};


#endif // !defined(AFX_INTERPRETER_H__4E868C25_AF96_41D5_A44F_2A29EF89B36E__INCLUDED_)
