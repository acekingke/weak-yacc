#include "FirstSet.h"
#include <iostream> 

/*   C   F I R S T   S E T .   S E T   U N I O N   */
/*-------------------------------------------------------------------------
    Owner: keyuchang
 Copy right belong to keyuchang
 Date 2008-2-3
     
-------------------------------------------------------------------------*/
int CFirstSet::SetUnion(SET_TYPE& s1,  SET_TYPE& s2)
{
	int changes = 0;
    SET_TYPE::iterator i;
    for (i=s2.begin();i!=s2.end();i++)
	{
		 if ( (s1.insert(*i)).second )
		 {
			 changes++;

		 }
		 else 
		 {
		 }	
	}
	return changes;
}
/*   C   F I R S T   S E T . I N S E R T   */
/*-------------------------------------------------------------------------
    Owner: keyuchang
 Copy right belong to keyuchang
 Date 2008-2-3
     
-------------------------------------------------------------------------*/
bool CFirstSet::insert(int value)
{
	std::pair<SET_TYPE::iterator,bool> status;
	status = this->set.insert(value);
    return status.second;
}
CFirstSet::CFirstSet(int value)
{
	insert(value);
}