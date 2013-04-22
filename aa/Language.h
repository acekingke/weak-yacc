/*-------------------------------------------------------------------------
    
 Copy right belong to keyuchang
 Date 2008-2-2
-------------------------------------------------------------------------*/
#ifndef _LANGUAGE_
#define _LANGUAGE_
#include "test.h"

#include "rules.h"
#include "FirstSet.h"

/*   C   L A N G U A G E   */
/*-------------------------------------------------------------------------
    Owner: keyuchang
 Copy right belong to keyuchang
 Date 2008-2-2 
  注意:
  所有的symbol对象均在vector中，其余的均只能访问其指针或序号
  不可有其他副本.  
-------------------------------------------------------------------------*/
class CLanguage
{
public:
    //string 是待计算的序列
    bool FirstFromVect(vector<CSymbol*>& str,CFirstSet& vSet, int &changes);    
    void InsertSymbol(CSymbol& vSymbol);
    void InsertRules(CRules& vRules);
    bool ComputerFirstSet();
    CLanguage(){ gensymbol = CSymbol(0,"$"); symbol.push_back(gensymbol);};
    CLanguage(vector<CSymbol>& symbol,
        vector<CRules>& rules);
    void SetStartSymbol(CSymbol& start);
    CSymbol gensymbol;                  //genarate symbol is $
    vector<CSymbol> symbol;
    vector<CRules> rules;
    CSymbol *startsym;
    void CopyVect(vector<CSymbol*> &stDes, 
        vector<CSymbol*> &stSource);
private:
    void FirstAllTerm();// 由computerFirstSet调用
};

#endif
