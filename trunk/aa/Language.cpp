/*-------------------------------------------------------------------------
    
 Copy right belong to keyuchang
 Date 2008-2-2
-------------------------------------------------------------------------*/
#include "Language.h"
/*   C   L A N G U A G E .   I N S E R T   S Y M B O L   */
/*-------------------------------------------------------------------------
    Owner: keyuchang
 Copy right belong to keyuchang 
 Date 2008-2-2
    
-------------------------------------------------------------------------*/
void CLanguage::InsertSymbol(CSymbol& vSymbol)
{
    symbol.push_back(vSymbol);
}
/*   C   L A N G U A G E .   I N S E R T   R U L E S   */
/*-------------------------------------------------------------------------
    Owner: keyuchang
 Copy right belong to keyuchang
 Date 2008-2-2
    
-------------------------------------------------------------------------*/
void CLanguage::InsertRules(CRules & vRules)
{
    rules.push_back(vRules);
}
/*   C   L A N G U A G E .   C   L A N G U A G E   */
/*-------------------------------------------------------------------------
    Owner: keyuchang
 Copy right belong to keyuchang
 Date 2008-2-2
    
-------------------------------------------------------------------------*/
CLanguage::CLanguage(vector < CSymbol >& symbol, 
    vector < CRules >& rules )
{
    vector<CSymbol>::iterator i;
    vector<CRules >::iterator j;
    for ( i = symbol.begin(); i != symbol.end(); i++)
    {
        this->symbol.push_back(*i);
    }
    for ( j = rules.begin(); j != rules.end(); i++ )
    {
        this->rules.push_back(*j);
    }
}
/*   C   L A N G U A G E .   C O M P U T E R   F I R S T   S E T   */
/*-------------------------------------------------------------------------
    Owner: keyuchang
 Copy right belong to keyuchang
 Date 2008-2-2
    
-------------------------------------------------------------------------*/
bool CLanguage::ComputerFirstSet()
{
    FirstAllTerm();//找出所有terminate symbol 的first集合
	int changes = 1; //初始化为1
	bool isEpsi ;
    int rIndex  ; //rules index 
	//if there is any changes do it cycle
	while ( changes )
	{ 
		changes = 0;
		//for each rule choice A->X1X2X3...Xn
		for ( rIndex = 0; rIndex < this->rules.size(); rIndex++ )
		{
                   isEpsi = FirstFromVect((this->rules[rIndex]).RightPart,
                        (this->rules[rIndex]).LeftPart->FirstSet, changes);
                   
                   (this->rules[rIndex]).LeftPart->setEpsilon(isEpsi);
		}
    }
    return false;
}
/*   C   L A N G U A G E .   S E T   S T A R T   S Y M B O L   */
/*-------------------------------------------------------------------------
    Owner: keyuchang
 Copy right belong to keyuchang
 Date 2008-2-3

    注意:仅当vn和vt设置完毕后才可以设置startsymbol,始终保持仅有一份拷贝
-------------------------------------------------------------------------*/
void CLanguage::SetStartSymbol(CSymbol& start)
{
    this->startsym = &start;
}

/*   C   L A N G U A G E .   F I R S T   A L L   T E R M   */
/*-------------------------------------------------------------------------
    Owner: keyuchang
 Copy right belong to keyuchang
 Date 2008-2-4 
    
-------------------------------------------------------------------------*/
void CLanguage::FirstAllTerm()
{
    vector<CSymbol>::iterator i;
	for ( i = symbol.begin(); i != symbol.end(); i++ )
	{
		if ( !(i->isNT) )//是非终结符
		{
			i->FirstSet.insert(i->value);
		}
	}
}

/*   C   L A N G U A G E .   F I R S T   F R O M   V E C T   */
/*-------------------------------------------------------------------------
    Owner: keyuchang
 Copy right belong to keyuchang
 Date 2008-2-4
    Function:from vect get the firstset of string,and then union it
             to vSet
    return : if epsilon in it ,return ture,otherwise return false
	changes : mean that any first is changing...
-------------------------------------------------------------------------*/
bool  CLanguage::FirstFromVect(vector<CSymbol*>& str,CFirstSet& vFirst,
							   int& changes)
{
    int k = 1-1;
	int n = str.size();
	bool bContinue = true;
    while ( bContinue && k<= n-1 )
	{
		//add first(Xk)-epsilon to vFirst
		changes += vFirst.SetUnion( vFirst.set,  str[ k ]->FirstSet.set );
		//if epsilon NOT in First(Xk) then Continue = false
		if ( !(str[ k ]->isEposilon) )
		{
			bContinue = false;
		}
		k = k + 1;
	}
    return bContinue;//from the vector we know wether it has epsilon
}
/*   C   L A N G U A G E .   C O P Y   V E C T   */
/*-------------------------------------------------------------------------
    Owner: keyuchang
 Copy right belong to keyuchang
 Date 2008-2-4
    功能:将stSource 复制到stDes,注意使用该函数之前
         保证stDes为空
-------------------------------------------------------------------------*/
void CLanguage::CopyVect(vector < CSymbol * > & stDes,
    vector < CSymbol * > & stSource)
{
	vector < CSymbol * >::iterator i;
	for ( i = stSource.begin(); i != stSource.end(); i++)
	{
		stDes.push_back(*i);
	}
}