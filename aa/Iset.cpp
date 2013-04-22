// Iset.cpp: implementation of the CIset class.
//
//////////////////////////////////////////////////////////////////////

#include "Iset.h"
#include <iostream>
extern void dispItem(CLanguage& L, CItem &vitem);
void disp(CLanguage& L,vector<CItem>& V)
{
      for (int i = 0; i< V.size(); i++)
      {
          dispItem(L, V[i]);
      }
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*   C   I S E T .   C   I S E T   */
/*-----------------------------------------------------------
    Owner: keyuchang
 Copy right belong to keyuchang
 Date 2008-2-16
    
--------------------------------------------------------------*/
CIset::CIset()
{

}
CIset::CIset(CItem firstItem)
{
     COREITEMTYPE aCore ;
     CItem aItem ;
     aItem.Dot = firstItem.Dot;
     aItem.RuleInd = firstItem.RuleInd;
    aItem.precs.insert(0);//�������ɷ���"$"
    aCore.coreItem.push_back(aItem);        
    this->Icore.push_back(aCore);

}
/*   C   I S E T .   C O M P U T E   A L L   C O R E   */
/*-----------------------------------------------------------
    Owner: keyuchang
 Copy right belong to keyuchang
 Date 2008-2-16
    
--------------------------------------------------------------*/
void CIset::ComputeAllCore(CLanguage & L, vector <COREITEMTYPE> & vIcore)//IcoreΪ���ļ�
{
//--------------+�㷨 +
//
// a)����ĿS'->.SΪ��̬��Ŀ���ĺ�,Ȼ�������հ�,closure(S'->.S)��̬��Ŀ��
    int changes = 1;
    int CoreInd = 0;
// b)�Գ�̬����������������Ŀ��Ӧ��GO(I,X) = closure(J)�õ�J
//�ظ�b,ֱ�����ٳ����µ���Ŀ��Ϊֹ 
   while (changes)
   {
       changes = 0;
       for ( CoreInd = 0; CoreInd< vIcore.size(); CoreInd++)
       {
           changes += UnionCore(L,vIcore,CoreInd);
       }
   }
} 
/*   C   I S E T .   U N I O N   C O R E   */
/*-----------------------------------------------------------
    Owner: keyuchang
 Copy right belong to keyuchang
 Date 2008-2-17
    �ҵ��µĺ�����Ŀ����
    vIcore,������precs,���õ�ת������
--------------------------------------------------------------*/
int CIset::UnionCore(CLanguage &L,vector <COREITEMTYPE> &vIcore, int CoreInd)
{
    int changes = 0;
    vector<CItem> Ivec;
    CSymbol* Sy;
    int newCoInd = -1;
    int i,j = 0;
    COREITEMTYPE& aCore = vIcore[CoreInd];
    //��һ��vIcore��ȡ��coreInd��Ԫ��,������бհ�����,������precs��ÿ���հ�Ԫ�ؼ���Go,
    //���������item��vIcore��û�еģ���ô����룬�������
    //Ϊ��֧�ֶ������Item,�������޸������������
    //aCore.coreItem.ComputerEClosure( L,  Ivec);
    //Ivec[0].ComputerEPrecs(L,Ivec);//����precs
    ComputerE(aCore,Ivec,L);
    GO ago;
    for (  i = 0; i < Ivec.size(); i++)
    {   
        CItem oCoreItem;
        COREITEMTYPE oCore ;
        oCore.coreItem.clear();
        if ( NULL != (Sy= Ivec[i].Go(oCoreItem, L)))//�������X
        { 
            //���Sy��vIcore[CoreInd].goVec�У�˵������һ�����Ų���
            //���������Ŀ�����Լӵ����е�״̬�� �У�changesҲҪ��1
            //�����޸�AddCoreNeeded��������֧�ָù���,���ҽ�Sy��ֵΪNULL,
            //�����ټ���GoVec
            oCore.coreItem.push_back(oCoreItem);
            //����ĺ�����֤���ͬһ���Ų�����������״̬
            //���޷���֤���ͬһ���Ų������ת����ͷ
            changes += AddCoreNeeded(vIcore,oCore,newCoInd,Sy,CoreInd);
            //GO ago(Sy,newCoInd);
            //�����������bug
#if 0
            for (int k = 0; k <vIcore[CoreInd].goVec.size();  k++)
            {
                if ( Sy == vIcore[CoreInd].goVec[k].first )
                {
                    Sy = NULL; 
                    break; 
                }
            }
#endif
            if ( -1 != newCoInd && Sy != NULL)
            {
                ago.first = Sy; 
                ago.second = newCoInd;
                //aCore�Ѿ������ı䣬��������ʹ���ˣ�����Ұָ������
                //if a new ,add ago in vIcore[CoreInd].govec
                if (checkNew(vIcore[CoreInd].goVec,ago)) 
                {
                    vIcore[CoreInd].goVec.push_back(ago);
                    changes++;
                }
            }
        }
    }
    return changes;
}
/*   C   I S E T .   A D D   C O R E   N E E D E D   */
/*-----------------------------------------------------------
    Owner: keyuchang
 Copy right belong to keyuchang
 Date 2008-2-18
    ���ַ����޷���⵽�����Go����
--------------------------------------------------------------*/
int CIset::AddCoreNeeded(vector<COREITEMTYPE> &vIcore,
                         COREITEMTYPE& core,int& newCoInd/*����core��vIcore�����*/,
                        CSymbol* &Sy, int index )
{
    int changes = 0;
    bool bFresh = true;//�Ƿ�����
    int i;
    int Ind = -1;
    bool needBoast = true;//�Ƿ���Ҫ�㲥
    int CInd = -1;
    //����goVec����Sy,����У��ӵ����е�core�У�����Sy = NULL, changes++,������
    //
    for ( int k = 0; k < vIcore[index].goVec.size(); k++ )
    {
        if ( vIcore[index].goVec[k].first == Sy 
            &&!checkExist(vIcore[vIcore[index].goVec[k].second],  core, CInd))
        {
            vIcore[vIcore[index].goVec[k].second].coreItem.push_back(core.coreItem[0]);
            Sy = NULL;
            changes++;
            return changes;
        }
        //�޸Ĵ��󴫵�precs ������,�����ٽ�precs ת������
        //״̬��
        else if ( vIcore[index].goVec[k].first == Sy
            &&checkExist(vIcore[vIcore[index].goVec[k].second],  core, CInd)
            &&-1 != CInd)
        {
            needBoast = false;//�����������򴫲�precs
            //��������ȷ����
            changes +=
            vIcore[vIcore[index].goVec[k].second].coreItem[CInd].precs.SetUnion(
                vIcore[vIcore[index].goVec[k].second].coreItem[CInd].precs.set,
                core.coreItem[0].precs.set);
            Sy = NULL;
            bFresh = false;//���ٲ���
        }
    }
    for ( i = 0; i < vIcore.size(); i++ )
    {
 //       if (core.coreItem.Dot == vIcore[i].coreItem.Dot
 //           && core.coreItem.RuleInd == vIcore[i].coreItem.RuleInd)
 // ��Ϊif core �Ѵ���
        if ( needBoast&&checkExist(vIcore[i],core,Ind) && Ind != -1)
        {
            bFresh = false;
            newCoInd = i;//ָ��ɵģ�ͬʱҲҪ��precs������
            //���ҵ�vIcore[i]�ж�Ӧ��core��ͬ����Ŀ�ſ��Բ���
            changes += vIcore[i].coreItem[Ind].precs.SetUnion(vIcore[i].coreItem[Ind].precs.set,
                core.coreItem[0].precs.set);
            break;
        }
    } 
    if ( bFresh )
    {
        vIcore.push_back(core);
        changes++;//�Ѿ��ı�
        newCoInd = i;//���ʣ�ָ����Item
    }
    return changes;
}
//Ҫ��������Ӧ��Ŀ���±�
bool CIset::checkExist(COREITEMTYPE& Icore,COREITEMTYPE& core,int& Ind)
{
 //       if (core.coreItem.Dot == vIcore[i].coreItem.Dot
 //           && core.coreItem.RuleInd == vIcore[i].coreItem.RuleInd)
    int i = 0;
    for ( i = 0; i < Icore.coreItem.size(); i++ )
    {
        if ( Icore.coreItem[ i ].Dot == core.coreItem[0].Dot
            && Icore.coreItem[ i ].RuleInd == core.coreItem[ 0 ].RuleInd )
        {
            Ind = i;//����ind,ָʾӦ�ô����ĸ�item
            return true;
        }
    }
    return false;
}
bool CIset::checkNew(vector < GO > & goVec, GO & ago)
{
    for ( int i = 0 ; i < goVec.size() ; i++)
    {
        if (ago == goVec[i]) return false;
    }
    return true;
}
//���ڼ���ÿ��������Ŀ�ıհ�item��precs
void CIset::ComputerE(COREITEMTYPE &core,vector<CItem>& Ivec,CLanguage& L)
{
    //Ϊ��֧�ֶ������Item,�������޸������������
    //aCore.coreItem.ComputerEClosure( L,  Ivec);
    //Ivec[0].ComputerEPrecs(L,Ivec);//����precs
    //����item���㣬ֻҪchanges != 0 ��Ҫ����
    //while (changes)
    //{
    //   changes = 0
    //   changes += ����
    //}
    int changes = 1;
    int i = 0;
    //��ʼ��
    for ( i = 0; i < core.coreItem.size(); i++ )
    {
        Ivec.push_back(core.coreItem[i]);
    }
    vector<CItem> Vanother;
    while (changes)
    {
        changes = 0;
            for ( i = 0; i < Ivec.size(); i++ )
            {
                Vanother.clear();
                Ivec[i].ComputerEClosure(L,Vanother);
                Vanother[0].ComputerEPrecs(L,Vanother);
                changes += Union2Core(Vanother,Ivec);
            }
        }
    

}
int CIset::Union2Core(vector<CItem>& Vanother, vector<CItem> &Ivec)
{
    int changes = 0;
    for ( int i = 0; i < Vanother.size(); i++)
    {
         for ( int j = 0; j < Ivec.size(); j++)
         { 
             if ( Vanother[ i ].Dot == Ivec[j].Dot 
                 && Vanother[i].RuleInd == Ivec[j].RuleInd )//�ҵ���ͬ��item
             {
                 changes += 
                     Ivec[j].precs.SetUnion(Ivec[j].precs.set,Vanother[i].precs.set);
                 break;
             }
             else 
             {
                 continue;
             }
         } 
         if ( j >= Ivec.size() )
         {
             Ivec.push_back(Vanother[i]);
             changes++;
         }
    }
    return changes;
}
CIset::~CIset()
{

}
