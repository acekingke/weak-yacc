#include "rules.h"
/*   C   R U L E S . I N S E R T   */
/*-------------------------------------------------------------------------
    Owner: keyuchang
 Copy right belong to keyuchang
 Date 2008-2-2
    插入一个右部符号
-------------------------------------------------------------------------*/
void CRules::insert(CSymbol *RightSymbol)
{
    this->RightPart.push_back(RightSymbol);
}
/*   C   R U L E S .   C   R U L E S   */
/*-------------------------------------------------------------------------
    Owner: keyuchang
 Copy right belong to keyuchang
 Date 2008-2-2 
    
-------------------------------------------------------------------------*/
CRules::CRules(CSymbol *LeftPart,vector<CSymbol*>&
               RP)
{
    vector<CSymbol*>::iterator i;
    this->LeftPart = LeftPart;
    for ( i = RP.begin(); i!=RP.end(); i++)
    {
        insert(*i);
    }
    this->LeftPart->setNT(true);
    if ( (this->RightPart).empty() )
    {
        this->LeftPart->setEpsilon( true );
    }
}