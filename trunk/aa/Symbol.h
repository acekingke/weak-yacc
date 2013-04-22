/*-------------------------------------------------------------------------
    
 Copy right belong to keyuchang
 Date 2008-2-2
-------------------------------------------------------------------------*/
#ifndef _SYMBOL_
#define _SYMBOL_
#include <string>
#include "FirstSet.h"
#include "test.h"

/*   C   S Y M B O L   */
/*-------------------------------------------------------------------------
    Owner: keyuchang
 Copy right belong to keyuchang
 Date 2008-2-2
    
-------------------------------------------------------------------------*/
class CSymbol
{ 
public:
    int value;
    std::string name ;
    CSymbol(){};
    CSymbol( int value, 
        std::string name, bool isNT = false ,
        bool isEposilon = false );
    void setEpsilon(bool isEposilon);
    void setNT(bool isNT);
    CFirstSet FirstSet;
    bool isNT;//如果是非终结符，其值为true,否则为false
    bool isEposilon;
};
#endif