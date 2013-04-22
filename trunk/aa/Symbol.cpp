//-------------------------------------------------------------------------
    
// Copy right belong to keyuchang
// Date 2008-2-2
//-------------------------------------------------------------------------*/

#include "Symbol.h"
//*   C   S Y M B O L .   C   S Y M B O L   */
//*-------------------------------------------------------------------------
//    Owner: keyuchang
// Copy right belong to keyuchang
// Date 2008-2-2
    
//-------------------------------------------------------------------------*/
CSymbol::CSymbol( int value, 
        std::string name, bool isNT  ,
        bool isEposilon  )
{
    this->value      = value;
    this->name       = name; 
    this->isNT       = isNT;
    this->isEposilon = isEposilon;
}

/*   C   S Y M B O L . S E T   E P S I L O N   */
/*-------------------------------------------------------------------------
    Owner: keyuchang
 Copy right belong to keyuchang
 Date 2008-2-2
    
-------------------------------------------------------------------------*/
void CSymbol::setEpsilon(bool isEposilon)
{
    this->isEposilon = isEposilon;
}

/*   C   S Y M B O L . S E T   N   T   */
/*-------------------------------------------------------------------------
    Owner: keyuchang
 Copy right belong to keyuchang
 Date 2008-2-2
    当符号为左部时必为NT
-------------------------------------------------------------------------*/
void CSymbol::setNT(bool isNT)
{
    this->isNT = isNT;
}
