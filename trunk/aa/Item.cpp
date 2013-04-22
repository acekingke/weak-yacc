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
���ȱ�֤waiter �Ƿ��ս��
--------------------------------------------------------------*/
void CItem::getAfterWaiter(CLanguage & L, vector < CSymbol * > & vect)
{
     CRules& r = L.rules[this->RuleInd];
    if ( Dot >= r.RightPart.size()) return ;//Dot ���޷���
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
    ע�⣺�ú���Ĭ��Ivec ��һ��Ԫ��Ϊ������Ŀ
    ������ΪItem Ϊ������Ŀ
--------------------------------------------------------------*/
void CItem::ComputerEPrecs(CLanguage &L, vector<CItem> &Ivec)
{
    //---------�㷨---------------
    /*
    Item ��precs ��֪����k��
    ���ÿ�����з���a ,����A->��.B�¼���
    ÿ��B->.�����first(��a)
    */
    CRules &r = L.rules[this->RuleInd];
    SET_TYPE::iterator i;//ָʾprecs
    vector<CSymbol*> syvec, beltaVec;
    
    int changes = 0;
    CFirstSet  vFirst;
    int j = 0;
    for ( i = this->precs.set.begin(); i != this->precs.set.end(); i++ )
    {
        syvec.clear();
        beltaVec.clear();
        this->getAfterWaiter(L,beltaVec);//ȡ�æ�
        syvec.push_back(&(L.symbol[*i]));//ȡ��a
        L.CopyVect(beltaVec, syvec);//�æ�a
        //first(��a)
        L.FirstFromVect(beltaVec, vFirst,  changes);//�ó�first(belta a)
        //�������е�B->.����
        //������Ŀ �������������ıհ���exp->.exp+term
        //���LeftPart == LeftPart At Dot,Dot��Ϊ0
        if ( Ivec[0].Dot == 0 && r.LeftPart == r.RightPart[Dot] )
        {
            Ivec[0].precs.SetUnion(Ivec[0].precs.set, vFirst.set);
        }
        for ( j = 1; j < Ivec.size(); j++ )//0�±��Ǻ�����Ŀ,����Ҫ��1��ʼ
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
    ע�⣺ֻ�к�����Ŀ�ſ���ʹ�øú���
--------------------------------------------------------------*/
void CItem::ComputerEClosure(CLanguage & L, vector < CItem > & Ivec)
{
    int changes = 1;
/*/-------------------�㷨--------*/
//a) I ����Ŀ��closesure ��
    Ivec.push_back(*this);// I ��Ϊ������Ŀ A->��.B��
//b)���A->��.B������closure(I),��B->.����ĿҲ����closure(I)
//c)�ظ�b)ֱ������������ĿΪֹ

    while (changes)
    {
        changes = 0;
        changes += UnionClosure(Ivec,L);//ֱ�����е�û�б仯
    }


}
/*   C   I T E M . G E T   R U L E   */
/*-----------------------------------------------------------
    Owner: keyuchang
 Copy right belong to keyuchang
 Date 2008-2-13 
    ��ʼ״̬��section Ϊ0����ʾ�ϵ㣬ͨ�����ƶϵ�
    �ɰ�Ҫ������ȡ��Rules
--------------------------------------------------------------*/
int CItem::getRule(CSymbol * LeftPart, CLanguage & L, int& section/*�ϵ�*/)
{
    for ( ; section < L.rules.size(); )
    {
        if ( L.rules[section].LeftPart == LeftPart ){ return section++;}
        section++;
    }
    return -1;
}
//---------------------------------------------------
// ����{��Ivec�в���B->.����Ŀ} ����Ivec��������Ŀ��������������Ŀ
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
     //����ҵ�һ��item������ʣ����룬����changes�Լ�1
     //�������
     //ֱ����Ҳ�Ҳ���
     for ( int i = 0; i < Ivec.size(); i++)
     {
         if ( Ivec[i].Dot >= L.rules[Ivec[i].RuleInd].RightPart.size()) //Dot�ǹ�Լ����
         { continue;}
          //��rules���ڵ�dot λ�ò��Ƿ��ս��    
         else if ( !(L.rules[Ivec[i].RuleInd].RightPart[Ivec[i].Dot]->isNT))
         {continue;}
         else //���������Ҫ���i���������item
         {
             while (-1 != (ruleInd=getRule(L.rules[Ivec[i].RuleInd].RightPart[Ivec[i].Dot],L,
                 section)))
             {
                   aItem.setRuleInd(ruleInd);
                   aItem.setDot( 0 );
                   if (checkFresh(aItem,Ivec))//����Ƿ�Ϊ��������
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
    Go(I,X) = J, ����X, ��J, JΪһ��������Ŀ,ͬʱ��precs
    ������ȥ
--------------------------------------------------------------*/
CSymbol*   CItem::Go( CItem & goItem,CLanguage &L)
{
// �㷨:
//*this �ǰ�����I �е�һ����Ŀ
//A->��.X�¶�Ӧthis, goItem��ӦA->��X.��
    CRules& r = L.rules[this->RuleInd];
    if ( Dot >= r.RightPart.size()) return NULL ;//Dot ���޷���
    goItem.setDot( Dot+1 );
    goItem.setRuleInd( RuleInd );
//����this��precs ���Ƹ�goItem
    goItem.precs.SetUnion(goItem.precs.set, precs.set);
    return r.RightPart[Dot];
}
