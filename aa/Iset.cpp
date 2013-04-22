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
    aItem.precs.insert(0);//插入生成符号"$"
    aCore.coreItem.push_back(aItem);        
    this->Icore.push_back(aCore);

}
/*   C   I S E T .   C O M P U T E   A L L   C O R E   */
/*-----------------------------------------------------------
    Owner: keyuchang
 Copy right belong to keyuchang
 Date 2008-2-16
    
--------------------------------------------------------------*/
void CIset::ComputeAllCore(CLanguage & L, vector <COREITEMTYPE> & vIcore)//Icore为核心集
{
//--------------+算法 +
//
// a)置项目S'->.S为初态项目集的核,然后对其求闭包,closure(S'->.S)初态项目集
    int changes = 1;
    int CoreInd = 0;
// b)对初态集或其他所构造项目集应用GO(I,X) = closure(J)得到J
//重复b,直到不再出现新的项目集为止 
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
    找到新的核心项目并入
    vIcore,并传播precs,并得到转换函数
--------------------------------------------------------------*/
int CIset::UnionCore(CLanguage &L,vector <COREITEMTYPE> &vIcore, int CoreInd)
{
    int changes = 0;
    vector<CItem> Ivec;
    CSymbol* Sy;
    int newCoInd = -1;
    int i,j = 0;
    COREITEMTYPE& aCore = vIcore[CoreInd];
    //从一个vIcore中取出coreInd的元素,对其进行闭包运算,并计算precs从每个闭包元素计算Go,
    //如果产生的item是vIcore中没有的，那么添加入，否则不添加
    //为了支持多个核心Item,还必须修改下面两个语句
    //aCore.coreItem.ComputerEClosure( L,  Ivec);
    //Ivec[0].ComputerEPrecs(L,Ivec);//计算precs
    ComputerE(aCore,Ivec,L);
    GO ago;
    for (  i = 0; i < Ivec.size(); i++)
    {   
        CItem oCoreItem;
        COREITEMTYPE oCore ;
        oCore.coreItem.clear();
        if ( NULL != (Sy= Ivec[i].Go(oCoreItem, L)))//如果存在X
        { 
            //如果Sy在vIcore[CoreInd].goVec中，说明它是一个符号产生
            //多个核心项目，所以加到已有的状态集 中，changes也要加1
            //建议修改AddCoreNeeded函数，以支持该功能,并且将Sy赋值为NULL,
            //以免再加入GoVec
            oCore.coreItem.push_back(oCoreItem);
            //下面的函数保证针对同一符号不会产生多余的状态
            //但无法保证针对同一符号产生多个转换箭头
            changes += AddCoreNeeded(vIcore,oCore,newCoInd,Sy,CoreInd);
            //GO ago(Sy,newCoInd);
            //以下语句解决该bug
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
                //aCore已经发生改变，不可以再使用了，属于野指针问题
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
    这种方法无法检测到自身的Go函数
--------------------------------------------------------------*/
int CIset::AddCoreNeeded(vector<COREITEMTYPE> &vIcore,
                         COREITEMTYPE& core,int& newCoInd/*返回core在vIcore的序号*/,
                        CSymbol* &Sy, int index )
{
    int changes = 0;
    bool bFresh = true;//是否新鲜
    int i;
    int Ind = -1;
    bool needBoast = true;//是否需要广播
    int CInd = -1;
    //查找goVec有无Sy,如果有，加到现有的core中，设置Sy = NULL, changes++,并返回
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
        //修改错误传递precs 的问题,即不再将precs 转向错误的
        //状态中
        else if ( vIcore[index].goVec[k].first == Sy
            &&checkExist(vIcore[vIcore[index].goVec[k].second],  core, CInd)
            &&-1 != CInd)
        {
            needBoast = false;//不再往错误方向传播precs
            //而传向正确方向
            changes +=
            vIcore[vIcore[index].goVec[k].second].coreItem[CInd].precs.SetUnion(
                vIcore[vIcore[index].goVec[k].second].coreItem[CInd].precs.set,
                core.coreItem[0].precs.set);
            Sy = NULL;
            bFresh = false;//不再插入
        }
    }
    for ( i = 0; i < vIcore.size(); i++ )
    {
 //       if (core.coreItem.Dot == vIcore[i].coreItem.Dot
 //           && core.coreItem.RuleInd == vIcore[i].coreItem.RuleInd)
 // 改为if core 已存在
        if ( needBoast&&checkExist(vIcore[i],core,Ind) && Ind != -1)
        {
            bFresh = false;
            newCoInd = i;//指向旧的，同时也要把precs传过来
            //先找到vIcore[i]中对应与core相同的项目才可以操作
            changes += vIcore[i].coreItem[Ind].precs.SetUnion(vIcore[i].coreItem[Ind].precs.set,
                core.coreItem[0].precs.set);
            break;
        }
    } 
    if ( bFresh )
    {
        vIcore.push_back(core);
        changes++;//已经改变
        newCoInd = i;//新鲜，指向新Item
    }
    return changes;
}
//要返回所对应项目的下标
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
            Ind = i;//设置ind,指示应该传递哪个item
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
//用于计算每个核心项目的闭包item和precs
void CIset::ComputerE(COREITEMTYPE &core,vector<CItem>& Ivec,CLanguage& L)
{
    //为了支持多个核心Item,还必须修改下面两个语句
    //aCore.coreItem.ComputerEClosure( L,  Ivec);
    //Ivec[0].ComputerEPrecs(L,Ivec);//计算precs
    //挨个item计算，只要changes != 0 就要继续
    //while (changes)
    //{
    //   changes = 0
    //   changes += 计算
    //}
    int changes = 1;
    int i = 0;
    //初始化
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
                 && Vanother[i].RuleInd == Ivec[j].RuleInd )//找到相同的item
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
