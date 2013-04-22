//--------------------------------------------------------------------------
// Copy right belong to keyuchang
// Date 2008-2-8
//--------------------------------------------------------------------------

#if !defined(_ISET_)
#define _ISET_
#include "Item.h"
#include <utility>
#include "test.h"
typedef std::pair<CSymbol*,int> GO;//GO(I,X)=J, pair�б�ʾX ��J
 class  COREITEMTYPE
{
 public:
     vector<CItem> coreItem;  // I ΪcoreItem
     vector<GO> goVec;//JΪcoreItem ��Iset�е����--��0��ʼ��XΪ����
     COREITEMTYPE(){};
     ~COREITEMTYPE(){};
 } ; 

class CIset  
{

public:
    vector <COREITEMTYPE> Icore; 
    CIset();
    CIset(CItem firstItem);
    virtual ~CIset();
    void ComputeAllCore(CLanguage& L,vector <COREITEMTYPE>& vIcore);
    int //����changes
        UnionCore(CLanguage &L,vector <COREITEMTYPE>& vIcore,int CoreInd);//�ҵ��µĺ�����Ŀ����vIcore,������precs,���õ�ת������
    void ComputerE(COREITEMTYPE& core,vector<CItem>& Ivec,CLanguage &L);
private:
    //Add core what's needed, if needed,Add and return 1, otherwise return 0
    int AddCoreNeeded(vector<COREITEMTYPE> &vIcore, COREITEMTYPE& core,
        int& newCoInd,CSymbol* &Sy, int index);
    bool checkNew( vector<GO>& goVec, GO& ago);
    bool checkExist(COREITEMTYPE &Icore, COREITEMTYPE& core, int &Ind);
    int //����changes
        Union2Core(vector<CItem>& Vanother,vector<CItem>& Ivec);
};  

#endif // !defined(_ISET_)
