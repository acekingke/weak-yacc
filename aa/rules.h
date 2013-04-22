/*-------------------------------------------------------------------------
    
 Copy right belong to keyuchang
 Date 2008-2-2
-------------------------------------------------------------------------*/
#ifndef _RULES_
#define _RULES_
//
#include "test.h"
#include <iostream>
#include <vector>
//#define MAXLEN_OF_RULE
#include "Symbol.h"

/*   C   R U L E S   */
/*-------------------------------------------------------------------------
    Owner: keyuchang
 Copy right belong to keyuchang
 Date 2008-2-2 
     
-------------------------------------------------------------------------*/
class CRules
{
public:
    CSymbol *LeftPart;
    vector<CSymbol*> RightPart;
    int RuleInd;
    CRules(){};
    CRules(CSymbol *LeftPart,
        vector<CSymbol*>& RP);
private:
    void insert(CSymbol *RightSymbol);
};

#endif