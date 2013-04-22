/*-------------------------------------------------------------------------
    
 Copy right belong to keyuchang
 Date 2008-2-6
-------------------------------------------------------------------------*/
// Item.h: interface for the CItem class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _ITEM_
#define _ITEM_
#include "Language.h"

/*   C   I T E M   */
/*-----------------------------------------------------------
    Owner: keyuchang
 Copy right belong to keyuchang 
 Date 2008-2-10 
    
--------------------------------------------------------------*/
class CItem  
{
public:
    int RuleInd;//项目所需的rule
    int Dot;    //圆点位置
    int Isetind ;// 所在的项目集序号
    CFirstSet precs; //先行符号集 
    CItem();
    CItem(int RuleInd, int Dot);
    virtual ~CItem();
    void getBeforeDot(CLanguage &L,vector<CSymbol*> &vect); 
 
    //waiter is the first unterminate symbol after dot
    void getAfterWaiter(CLanguage &L,vector<CSymbol*> &vect); 
    void ComputerEClosure(CLanguage &L, vector<CItem> &Ivec);
    void setRuleInd(int RuleInd);
    void setDot(int Dot);
    void setI(int Isetind);
    void ComputerEPrecs(CLanguage &L, vector<CItem> &Ivec);

    CSymbol* Go(CItem & goItem,CLanguage &L);//Go(I,X), return X,J注意同时将precs传播出去
private:
    int getRule(CSymbol* LeftPart, CLanguage &L,int& section);
    int UnionClosure(vector < CItem > & Ivec, CLanguage &L);
    bool checkFresh(CItem item, vector< CItem > &Ivec);
};

#endif // !defined(_ITEM_)
