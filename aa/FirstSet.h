/*-------------------------------------------------------------------------
    
 
    Owner: keyuchang
-------------------------------------------------------------------------*/
    
#ifndef _FIRST_SET
#define _FIRST_SET
#include <set> 
#pragma warning(disable:4786)
#include "test.h"
using namespace std;
typedef std::set<int> SET_INT, SET_TYPE;

/*   C   F I R S T   S E T   */
/*-------------------------------------------------------------------------
    Owner: keyuchang
 Copy right belong to keyuchang
    
-------------------------------------------------------------------------*/
class CFirstSet
{
public:
  SET_TYPE set;
  int SetUnion(SET_TYPE& s1,  SET_TYPE& s2);
  CFirstSet(int value);
  CFirstSet(){};
  bool insert(int value); 
};
#endif