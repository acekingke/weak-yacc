/*-------------------------------------------------------------------------
    
 Copy right belong to keyuchang
 Date 2008-2-6
-------------------------------------------------------------------------*/

#include "Item.h" 

/////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*   C   I T E M .   C   I T E M   */
/*-----------------------------------------------------------
    Owner: keyuchang
 Copy right belong to keyuchang
 Date 2008-2-15
    
--------------------------------------------------------------*/
CItem::CItem()
{
	this->Dot = 0;
	this->RuleInd = 0; 
}
CItem::CItem(int RuleInd, int Dot)
{
    setDot( Dot );
    setRuleInd( RuleInd );
}
CItem::~CItem()
{

}

/*   C   I T E M . G E T   B E F O R E   D O T   */
/*-----------------------------------------------------------
    Owner: keyuchang
 Copy right belong to keyuchang
 Date 2008-2-10
//use the function should make sure vect is empty
--------------------------------------------------------------*/
void CItem::getBeforeDot(CLanguage & L, vector < CSymbol * > & vect)
{
      CRules& r = L.rules[this->RuleInd];
     if ( 0 >= this->Dot ||  //dot before it has none or all
        this->Dot > r.RightPart.size() )
     {
         return ;
     }

     for ( int i = 0; i < this->Dot; i++)
     {
    	 vect.push_back(r.RightPart[i]);
     } 
}
/*   C   I T E M . G E T   A F T E R   W A I T E R   */
/*-----------------------------------------------------------
    Owner: keyuchang
 Copy right belong to keyuchang
 Date 2008-2-10
首先保证waiter 是非终结符
--------------------------------------------------------------*/
void CItem::getAfterWaiter(CLanguage & L, vector < CSymbol * > & vect)
{
     CRules& r = L.rules[this->RuleInd];
    if ( Dot >= r.RightPart.size()) return ;//Dot 后无符号
    if ( !(r.RightPart[Dot]->isNT) )
    {
        return;
    }
    for( int i = this->Dot + 1; i < r.RightPart.size(); i++ )
    {
        vect.push_back(r.RightPart[ i ]);
    }
    return ;
}
/*   C   I T E M . S E T   D O T   */
/*-----------------------------------------------------------
    Owner: keyuchang
 Copy right belong to keyuchang
 Date 2008-2-15
    
--------------------------------------------------------------*/
void CItem::setDot(int Dot)
{
    this->Dot = Dot;
}
/*   C   I T E M . S E T   R U L E   I N D   */
/*-----------------------------------------------------------
    Owner: keyuchang
 Copy right belong to keyuchang
 Date 2008-2-12
    
--------------------------------------------------------------*/
void CItem::setRuleInd(int RuleInd)
{
    this->RuleInd = RuleInd;
}
/*   C   I T E M . S E T   I   */
/*-----------------------------------------------------------
    Owner: keyuchang
 Copy right belong to keyuchang
 Date 2008-2-12
    
--------------------------------------------------------------*/
void CItem::setI(int Isetind)
{
    this->Isetind = Isetind;
}
/*   C   I T E M .   C O M P U T E R   P R E C S   */
/*-----------------------------------------------------------
    Owner: keyuchang
 Copy right belong to keyuchang
 Date 2008-2-15
    注意：该函数默认Ivec 第一个元素为核心项目
    并且认为Item 为核心项目
--------------------------------------------------------------*/
void CItem::ComputerEPrecs(CLanguage &L, vector<CItem> &Ivec)
{
    //---------算法---------------
    /*
    Item 的precs 已知，共k个
    针对每个先行符号a ,根据A->α.Bβ计算
    每个B->.γ添加first(βa)
    */
    CRules &r = L.rules[this->RuleInd];
    SET_TYPE::iterator i;//指示precs
    vector<CSymbol*> syvec, beltaVec;
    
    int changes = 0;
    CFirstSet  vFirst;
    int j = 0;
    for ( i = this->precs.set.begin(); i != this->precs.set.end(); i++ )
    {
        syvec.clear();
        beltaVec.clear();
        this->getAfterWaiter(L,beltaVec);//取得β
        syvec.push_back(&(L.symbol[*i]));//取得a
        L.CopyVect(beltaVec, syvec);//得βa
        //first(βa)
        L.FirstFromVect(beltaVec, vFirst,  changes);//得出first(belta a)
        //插入所有的B->.γ中
        //核心项目 自身可能是自身的闭包项exp->.exp+term
        //如果LeftPart == LeftPart At Dot,Dot必为0
        if ( Ivec[0].Dot == 0 && r.LeftPart == r.RightPart[Dot] )
        {
            Ivec[0].precs.SetUnion(Ivec[0].precs.set, vFirst.set);
        }
        for ( j = 1; j < Ivec.size(); j++ )//0下标是核心项目,所以要从1开始
        {
            Ivec[j].precs.SetUnion(Ivec[j].precs.set, vFirst.set);
        }
    }
}
/*   C   I T E M .   C O M P U T E R   E   C L O S U R E   */
/*-----------------------------------------------------------
    Owner: keyuchang
 Copy right belong to keyuchang
 Date 2008-2-15
    注意：只有核心项目才可以使用该函数
--------------------------------------------------------------*/
void CItem::ComputerEClosure(CLanguage & L, vector < CItem > & Ivec)
{
    int changes = 1;
/*/-------------------算法--------*/
//a) I 中项目在closesure 中
    Ivec.push_back(*this);// I 即为核心项目 A->α.Bβ
//b)如果A->α.Bβ属于closure(I),则B->.γ项目也属于closure(I)
//c)重复b)直到不出现新项目为止

    while (changes)
    {
        changes = 0;
        changes += UnionClosure(Ivec,L);//直到现有的没有变化
    }


}
/*   C   I T E M . G E T   R U L E   */
/*-----------------------------------------------------------
    Owner: keyuchang
 Copy right belong to keyuchang
 Date 2008-2-13 
    初始状态中section 为0，表示断点，通过控制断点
    可按要求逐条取出Rules
--------------------------------------------------------------*/
int CItem::getRule(CSymbol * LeftPart, CLanguage & L, int& section/*断点*/)
{
    for ( ; section < L.rules.size(); )
    {
        if ( L.rules[section].LeftPart == LeftPart ){ return section++;}
        section++;
    }
    return -1;
}
//---------------------------------------------------
// 函数{在Ivec中插入B->.γ项目} 发现Ivec中现有项目产生的所有新项目
// return changes
//------------------------------------------------
/*   C   I T E M .   U N I O N   C L O S U R E   */
/*-----------------------------------------------------------
    Owner: keyuchang
 Copy right belong to keyuchang
 Date 2008-2-15
    
--------------------------------------------------------------*/
int CItem::UnionClosure(vector < CItem > & Ivec, CLanguage &L)
{
     int changes = 0;
     int section = 0;
     int ruleInd = -1;
     CItem aItem;
     //如果找到一条item如果新鲜，插入，并且changes自加1
     //否则继续
     //直到再也找不到
     for ( int i = 0; i < Ivec.size(); i++)
     {
         if ( Ivec[i].Dot >= L.rules[Ivec[i].RuleInd].RightPart.size()) //Dot是规约句子
         { continue;}
          //或rules所在的dot 位置不是非终结符    
         else if ( !(L.rules[Ivec[i].RuleInd].RightPart[Ivec[i].Dot]->isNT))
         {continue;}
         else //其他情况均要针对i项插入新鲜item
         {
             while (-1 != (ruleInd=getRule(L.rules[Ivec[i].RuleInd].RightPart[Ivec[i].Dot],L,
                 section)))
             {
                   aItem.setRuleInd(ruleInd);
                   aItem.setDot( 0 );
                   if (checkFresh(aItem,Ivec))//检查是否为新鲜玩意
                   {
                       changes++;
                       Ivec.push_back(aItem);
                   }
             }
         }
     }
     return changes;
}
/*   C   I T E M . C H E C K   F R E S H   */
/*-----------------------------------------------------------
    Owner: keyuchang
 Copy right belong to keyuchang
 Date 2008-2-15
    
--------------------------------------------------------------*/
bool CItem::checkFresh(CItem item, vector< CItem > &Ivec)
{
    for (int i = 0; i < Ivec.size(); i++ )
    {
        if ( item.Dot == Ivec[i].Dot &&
             item.RuleInd == Ivec[i].RuleInd )
        { return false;}
    }
    return true;
}
/*   C   I T E M .   G O   */
/*-----------------------------------------------------------
    Owner: keyuchang
 Copy right belong to keyuchang
 Date 2008-2-16
    Go(I,X) = J, 返回X, 和J, J为一个核心项目,同时将precs
    传播出去
--------------------------------------------------------------*/
CSymbol*   CItem::Go( CItem & goItem,CLanguage &L)
{
// 算法:
//*this 是包含在I 中的一个项目
//A->α.Xβ对应this, goItem对应A->αX.β
    CRules& r = L.rules[this->RuleInd];
    if ( Dot >= r.RightPart.size()) return NULL ;//Dot 后无符号
    goItem.setDot( Dot+1 );
    goItem.setRuleInd( RuleInd );
//并将this的precs 复制给goItem
    goItem.precs.SetUnion(goItem.precs.set, precs.set);
    return r.RightPart[Dot];
}
