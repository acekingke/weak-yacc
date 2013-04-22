//--------------------------------------------------------------------------
// Copy right belong to keyuchang
// Date 2008-2-8
//--------------------------------------------------------------------------

#if !defined(_ISET_)
#define _ISET_
#include "Item.h"
#include <utility>
#include "test.h"
typedef std::pair<CSymbol*,int> GO;//GO(I,X)=J, pair中表示X 和J
 class  COREITEMTYPE
{
 public:
     vector<CItem> coreItem;  // I 为coreItem
     vector<GO> goVec;//J为coreItem 在Iset中的序号--从0开始，X为符号
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
    int //返回changes
        UnionCore(CLanguage &L,vector <COREITEMTYPE>& vIcore,int CoreInd);//找到新的核心项目并入vIcore,并传播precs,并得到转换函数
    void ComputerE(COREITEMTYPE& core,vector<CItem>& Ivec,CLanguage &L);
private:
    //Add core what's needed, if needed,Add and return 1, otherwise return 0
    int AddCoreNeeded(vector<COREITEMTYPE> &vIcore, COREITEMTYPE& core,
        int& newCoInd,CSymbol* &Sy, int index);
    bool checkNew( vector<GO>& goVec, GO& ago);
    bool checkExist(COREITEMTYPE &Icore, COREITEMTYPE& core, int &Ind);
    int //返回changes
        Union2Core(vector<CItem>& Vanother,vector<CItem>& Ivec);
};  

#endif // !defined(_ISET_)
