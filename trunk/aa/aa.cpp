#pragma warning(disable:4786)                                                           //                  //
#include <set>                                                                                              //
#include <iostream>
#include <string>
#include "FirstSet.h"
#include "Symbol.h"
#include "Language.h"
#include "Item.h"
#include "Iset.h"
#include "Interpreter.h"
//#define TEST 
using namespace std ;
typedef set<int> SET_INT, SET_TYPE;
//op1 = op1 + op2
//如果集合发生了改变，那么返回值不为0
//否则为0
extern void disp(CLanguage& L,vector<CItem>& V);

int SetUnion(SET_TYPE& s1,  SET_TYPE& s2)
{
    int changes = 0;
    SET_TYPE::iterator i;
    for (i=s2.begin();i!=s2.end();i++)
    {
         if ( (s1.insert(*i)).second )
         {
#ifdef  TEST
             cout<<"		insert"<<endl;
#endif
             changes++;

         }
         else 
         {
         }  
    }
    return changes;
}
#ifdef TEST 
void showsym(vector<CSymbol> &vSym)
{
    
    int i = 0;
    for ( i = 0; i<vSym.size(); i++)
	{
        cout<<vSym[i].name<<": eposilon is  "
        <<vSym[i].isEposilon<<" nt is "
        <<vSym[i].isNT<<" " 
		<<"value is "
		<<vSym[i].value
        <<endl;
	}
}
void showrule(CRules& r)
{
    cout<<"Rule: " ;
    cout<<r.LeftPart->name<<"->";
    for ( int i = 0; i < r.RightPart.size(); i++ )
        cout<<r.RightPart[i]->name<<" ";
    cout<<endl;
}
void showFirst(CSymbol &vsy, CLanguage& L)
{
    SET_TYPE::iterator i;
	cout<<vsy.name<<":"
        <<"firstset value is " ;
	for ( i = (vsy.FirstSet.set).begin();
	     i != (vsy.FirstSet.set).end(); i++)
	{
		cout<<(L.symbol[*i]).name<<" ";
	}
     if( vsy.isEposilon )
	 {
		 cout<<" epsilon ";
	 }
     cout<<endl;
}

void dispItem(CLanguage& L, CItem &vitem)
{
    int i = 0;
    CRules &r = L.rules[ vitem.RuleInd];
    cout<<r.LeftPart->name<<"->";
    for (  i = 0; i < vitem.Dot; i++ )//显示dot 之前的符号
      {  cout<<r.RightPart[i]->name<<" ";}
    cout<<"@";
    for ( i = vitem.Dot; i< r.RightPart.size(); i++)
    {
        cout<<r.RightPart[i]->name<<" " ;
    }
    cout<<endl;
    //display the precs
    SET_TYPE::iterator j;
	cout<<"The precs is :"; 
	for ( j = (vitem.precs.set).begin();
	     j != (vitem.precs.set).end(); j++)
	{
		cout<<(L.symbol[*j]).name<<" ";
	}
   
     cout<<endl;
}
void dispIcore(CIset& Iset, CLanguage &L)
{
     int i, j,k;
    for (  i =0 ;i < Iset.Icore.size(); i++)
    {
        cout<<"I"<<i<<":"<<endl;
        for ( k = 0; k < Iset.Icore[i].coreItem.size(); k++)
        {
            dispItem(L, Iset.Icore[i].coreItem[k]);
        }
        //显示Icore[i].goVec
        for (  j = 0; j < Iset.Icore[i].goVec.size(); j++ ) 
        {
            cout<<"     ."<<Iset.Icore[i].goVec[j].first->name;
            if (Iset.Icore[i].goVec[j].first->isNT)
                    cout<<" goto ";
            else
                  cout<<" shift ";
           cout<<Iset.Icore[i].goVec[j].second<<endl;
        }
    }
}

#else 
void showsym(vector<CSymbol> &vSym){}
void showFirst(CSymbol &vsy, CLanguage& L){}
void showrule(CRules& r){}
#endif
//test1
void test1()
{
	    CLanguage lang;

	//定义一个符号集合V
    CSymbol stmt_sequence(1,"stmt_sequence");
    CSymbol stmt(2,"stmt");
    CSymbol stmt_seq(3,"stmt_seq");
    CSymbol semicolon(4,";");
    CSymbol s(5,"s");

    //一.获得CLanguage之vector<CSymbol>
    vector<CSymbol> vSym;
    lang.InsertSymbol(stmt_sequence);//1
    lang.InsertSymbol(stmt);         //2 
    lang.InsertSymbol(stmt_seq);     //3
    lang.InsertSymbol(semicolon);    //4
    lang.InsertSymbol(s);            //5
    showsym(lang.symbol);                //显示之
    //二.获得CLanguage之vector<Rules>
    //1.获得rightpart之Vector<CSymbol*>
#define vlang(x) (&lang.symbol[x])
    //rule1
    vector<CSymbol*> rpart1;
    rpart1.push_back(vlang( 2 ));
       cout<<vlang(2)<<" part"<<endl;
       cout<<rpart1[0]<<endl;
    rpart1.push_back(vlang( 3 ));
    //2。获得leftpart--取得rules
    CRules r1(vlang(1),rpart1);
    //3.插入vector<Rules>
    showrule( r1 );

    //r2
    vector<CSymbol*> rpart2;
    rpart2.push_back(vlang(4));
    rpart2.push_back(vlang(1));
    CRules r2(vlang(3),rpart2);
    showrule( r2 );

    //r3
    vector<CSymbol*> rpart3;
    CRules r3(vlang(3),rpart3);
    showrule( r3 );

    //r4
    vector<CSymbol*> rpart4;
    rpart4.push_back(vlang(5));
    CRules r4(vlang(2),rpart4);
    showrule(r4);

    cout<<" \nThe language's symbol is "<<endl;
   
    showsym( lang.symbol );

    lang.InsertRules(r1);
    lang.InsertRules(r2);
    lang.InsertRules(r3);
    lang.InsertRules(r4);
    cout << "\nThe languge's rules is"<<endl; 
    for ( int i = 0; i<lang.rules.size(); i++)
       showrule(lang.rules[i]);
#undef vlang  

	//三.获得开始符号
	lang.SetStartSymbol(lang.symbol[1]);
	cout<<"\nlanguage's startsymbol is "<<lang.startsym->name
		<<endl;
	//language初始化结束

	//计算first集合
	//1.计算所有终结符的first集合
	//lang.FirstAllTerm();
    lang.ComputerFirstSet();
    for (  i = 0; i<lang.symbol.size(); i++)
       showFirst(lang.symbol[i],lang);
    //2.从rules中计算first集合
}
void test21()
{
        CLanguage lang;

    //定义一个符号集合V
    CSymbol C(1,"C");
    CSymbol A(2,"A");
    CSymbol B(3,"B");
    CSymbol c(4,"c");
    CSymbol a(5,"a");
    CSymbol b(6,"b");
    
    //一.获得CLanguage之vector<CSymbol>
    lang.InsertSymbol(C);         //1
    lang.InsertSymbol(A);         //2 
    lang.InsertSymbol(B);         //3
    lang.InsertSymbol(c);         //4
    lang.InsertSymbol(a);         //5    
    lang.InsertSymbol(b);         //6
    showsym(lang.symbol);                //显示之

    //二.获得CLanguage之vector<Rules>
    //1.获得rightpart之Vector<CSymbol*>
#define vlang(x) (&lang.symbol[x])
#if 0
   //rule1
    vector<CSymbol*> rpart1;
    rpart1.push_back(vlang( 2 ));
    rpart1.push_back(vlang( 3 ));
    rpart1.push_back(vlang( 4 ));
    //2。获得leftpart--取得rules
    CRules r1(vlang(1),rpart1);
    //3.插入vector<Rules>
    showrule( r1 );
 #endif 
 
    //r2
    vector<CSymbol*> rpart2;
    rpart2.push_back(vlang(2));
    rpart2.push_back(vlang(3));
    CRules r2(vlang(1),rpart2);
    showrule( r2 );

    //r3
    vector<CSymbol*> rpart3;
    rpart3.push_back(vlang(5));
    CRules r3(vlang(2),rpart3);
    showrule( r3 );

    //r4
    vector<CSymbol*> rpart4;
    CRules r4(vlang(2),rpart4);
    showrule(r4);
    
    //r5
    vector<CSymbol*> rpart5;
    rpart5.push_back(vlang(6));
    CRules r5(vlang(3),rpart5);
    showrule( r5 );

    //r6
    vector< CSymbol* > rpart6;
    CRules r6(vlang(3),rpart6);
    showrule( r6 );
    
    cout<<" \nThe language's symbol is "<<endl;
   
    showsym( lang.symbol );
//    lang.InsertRules(r1);
    lang.InsertRules(r2);
    lang.InsertRules(r3);
    lang.InsertRules(r4);
    lang.InsertRules(r5);
    lang.InsertRules(r6);
    
    cout << "\nThe languge's rules is"<<endl; 
    for ( int i = 0; i<lang.rules.size(); i++)
       showrule(lang.rules[i]);
#undef vlang 

    //三.获得开始符号
    lang.SetStartSymbol(lang.symbol[1]);
    cout<<"\nlanguage's startsymbol is "<<lang.startsym->name
        <<endl;
    //language初始化结束

    //计算first集合
    //1.计算所有终结符的first集合
    //lang.FirstAllTerm();
    lang.ComputerFirstSet();
    for (  i = 0; i<lang.symbol.size(); i++)
       showFirst(lang.symbol[i],lang);
    //2.从rules中计算first集合
}

void test8()
{
    CInterpreter inter;
    CLanguage lang;
    TokenType ty;
    while (END != (ty = inter.getToken(lang)))
    {
        if ( NAME == ty )
        {
            cout<<"     "<<inter.string_value<< " :"<<inter.strMap[inter.string_value]
                <<endl;
        }
    }
    cout<<endl;
    showsym( lang.symbol );
}
void test9()
{
    CInterpreter inter;
    CLanguage lang;
    inter.getRules(lang);
    showsym( lang.symbol );
    for ( int i = 0; i<lang.rules.size(); i++)
       showrule(lang.rules[i]);
}
void test10()
{
    CInterpreter inter;
    CLanguage lang;
    inter.getRules(lang);
    showsym( lang.symbol );
    for ( int i = 0; i<lang.rules.size(); i++)
       showrule(lang.rules[i]);
    lang.SetStartSymbol(lang.symbol[1]);
    cout<<"\nlanguage's startsymbol is "<<lang.startsym->name
    	<<endl;
    //language初始化结束

    //计算first集合
    //1.计算所有终结符的first集合
    //lang.FirstAllTerm();
    //2.从rules中计算first集合
    lang.ComputerFirstSet();
    for (  i = 0; i<lang.symbol.size(); i++)
       showFirst(lang.symbol[i],lang);

    //设置一个item 并计算其closure
    //I0
    CItem firstItem(0,0);
    CIset aIset(firstItem);
    aIset.ComputeAllCore(lang,aIset.Icore);
    dispIcore(aIset,lang);
}
void test11()
{
    CInterpreter inter;
    CLanguage lang;
    inter.getRules(lang);
    showsym( lang.symbol );
    for ( int i = 0; i<lang.rules.size(); i++)
       showrule(lang.rules[i]);
    lang.SetStartSymbol(lang.symbol[1]);
    cout<<"\nlanguage's startsymbol is "<<lang.startsym->name
    	<<endl;
    //language初始化结束

    //计算first集合
    //1.计算所有终结符的first集合
    //lang.FirstAllTerm();
    //2.从rules中计算first集合
    lang.ComputerFirstSet();
    for (  i = 0; i<lang.symbol.size(); i++)
       showFirst(lang.symbol[i],lang);

    //设置一个item 并计算其closure
    //I0
    CItem firstItem(0,0);
    CIset aIset(firstItem);
    aIset.ComputeAllCore(lang,aIset.Icore);
    dispIcore(aIset,lang);
    inter.FormAnsyTable(aIset, lang);
    cout<<"The action String count is"<<endl;
    cout<< inter.ActionVec.size()<<endl;
    for ( i = 0; i < inter.ActionVec.size(); i++)
    {
        cout<<i<<":"<<inter.ActionVec[i]<<endl;
    }

}
void test2()
{
	    CLanguage lang;

	//定义一个符号集合V
    CSymbol C(1,"C");
    CSymbol A(2,"A");
    CSymbol B(3,"B");
    CSymbol c(4,"c");
    CSymbol a(5,"a");
    CSymbol b(6,"b");
    
    //一.获得CLanguage之vector<CSymbol>
    lang.InsertSymbol(C);         //1
    lang.InsertSymbol(A);         //2 
    lang.InsertSymbol(B);         //3
    lang.InsertSymbol(c);         //4
    lang.InsertSymbol(a);         //5    
    lang.InsertSymbol(b);         //6
    showsym(lang.symbol);                //显示之

    //二.获得CLanguage之vector<Rules>
    //1.获得rightpart之Vector<CSymbol*>
#define vlang(x) (&lang.symbol[x])
   //rule1
    vector<CSymbol*> rpart1;
    rpart1.push_back(vlang( 2 ));
    rpart1.push_back(vlang( 3 ));
    rpart1.push_back(vlang( 4 ));
    //2。获得leftpart--取得rules
    CRules r1(vlang(1),rpart1);
    //3.插入vector<Rules>
    showrule( r1 );

    //r2
    vector<CSymbol*> rpart2;
    rpart2.push_back(vlang(2));
    rpart2.push_back(vlang(3));
    CRules r2(vlang(1),rpart2);
    showrule( r2 );

    //r3
    vector<CSymbol*> rpart3;
    rpart3.push_back(vlang(5));
    CRules r3(vlang(2),rpart3);
    showrule( r3 );

    //r4
    vector<CSymbol*> rpart4;
    CRules r4(vlang(2),rpart4);
    showrule(r4);
    
    //r5
    vector<CSymbol*> rpart5;
    rpart5.push_back(vlang(6));
    CRules r5(vlang(3),rpart5);
    showrule( r5 );

    //r6
    vector< CSymbol* > rpart6;
    CRules r6(vlang(3),rpart6);
    showrule( r6 );
    
    cout<<" \nThe language's symbol is "<<endl;
   
    showsym( lang.symbol );

    lang.InsertRules(r1);
    lang.InsertRules(r2);
    lang.InsertRules(r3);
    lang.InsertRules(r4);
    lang.InsertRules(r5);
    lang.InsertRules(r6);
    
    cout << "\nThe languge's rules is"<<endl; 
    for ( int i = 0; i<lang.rules.size(); i++)
       showrule(lang.rules[i]);
#undef vlang 

	//三.获得开始符号
	lang.SetStartSymbol(lang.symbol[1]);
	cout<<"\nlanguage's startsymbol is "<<lang.startsym->name
		<<endl;
	//language初始化结束

	//计算first集合
	//1.计算所有终结符的first集合
	//lang.FirstAllTerm();
    lang.ComputerFirstSet();
    for (  i = 0; i<lang.symbol.size(); i++)
       showFirst(lang.symbol[i],lang);
    //2.从rules中计算first集合
}
void test3()
{
	    CLanguage lang;

	//定义一个符号集合V
    CSymbol A1(1,"A1");//v1
    CSymbol A(2, "A");  //v2
    CSymbol LP(3,"("); //v3
    CSymbol RP(4,")");//v4
    CSymbol a  (5,"a");//v5
    //一.获得CLanguage之vector<CSymbol>
    lang.InsertSymbol(A1);         //1
    lang.InsertSymbol(A);
    lang.InsertSymbol(LP);         //3 
    lang.InsertSymbol(RP);         //4
    lang.InsertSymbol(a);         //5
    showsym(lang.symbol);                //显示之

    //二.获得CLanguage之vector<Rules>
    //1.获得rightpart之Vector<CSymbol*>
#define vlang(x) (&lang.symbol[x])
   //rule1
    vector<CSymbol*> rpart1;
    rpart1.push_back(vlang( 2 ));

    //2。获得leftpart--取得rules
    CRules r1(vlang(1),rpart1);
    //3.插入vector<Rules> 
    showrule( r1 );

    //r2
    vector<CSymbol*> rpart2;
    rpart2.push_back(vlang(3));
    rpart2.push_back(vlang(2));
    rpart2.push_back(vlang(4));
    CRules r2(vlang(2),rpart2);
    showrule( r2 );

    //r3
    vector<CSymbol*> rpart3;
    rpart3.push_back(vlang(5));
    CRules r3(vlang(2),rpart3);
    showrule( r3 );


    
    cout<<" \nThe language's symbol is "<<endl;
   
    showsym( lang.symbol );

    lang.InsertRules(r1);
    lang.InsertRules(r2);
    lang.InsertRules(r3);
    
    cout << "\nThe languge's rules is"<<endl; 
    for ( int i = 0; i<lang.rules.size(); i++)
       showrule(lang.rules[i]);
#undef vlang 

	//三.获得开始符号
	lang.SetStartSymbol(lang.symbol[1]);
	cout<<"\nlanguage's startsymbol is "<<lang.startsym->name
		<<endl;
	//language初始化结束

	//计算first集合
	//1.计算所有终结符的first集合
	//lang.FirstAllTerm();
    //2.从rules中计算first集合
    lang.ComputerFirstSet();
    for (  i = 0; i<lang.symbol.size(); i++)
       showFirst(lang.symbol[i],lang);
    
    //设置一个item 并计算其closure
    CItem item;
    vector < CItem >  Ivec;

    //I0
    item.setDot( 0 );
    item.setRuleInd( 0 );
    item.ComputerEClosure(lang ,Ivec);
    cout<<"\nI0:"<<endl; 
    for (  i = 0; i < Ivec.size(); i++)
    {
        dispItem( lang,  Ivec[i]);
    }

    //I1
    Ivec.clear();
    item.setDot(1);
    item.setRuleInd(0);
    item.ComputerEClosure(lang,Ivec);
    cout<<"\nI1:"<<endl;
    for (  i = 0; i < Ivec.size(); i++)
    {
        dispItem( lang,  Ivec[i]);
    }
    //I2
    Ivec.clear();
    item.setDot(1);
    item.setRuleInd(2);
    item.ComputerEClosure(lang,Ivec);
    cout<<"\nI2:"<<endl;
    for (  i = 0; i < Ivec.size(); i++)
    {
        dispItem( lang,  Ivec[i]);
    }
    //I3
    Ivec.clear();
    item.setDot(1);
    item.setRuleInd(1);
    item.ComputerEClosure(lang,Ivec);
    cout<<"\nI3:"<<endl;
    for (  i = 0; i < Ivec.size(); i++)
    {
        dispItem( lang,  Ivec[i]);
    }
    //I4
    Ivec.clear();
    item.setDot(2);
    item.setRuleInd(1);
    item.ComputerEClosure(lang,Ivec);
    cout<<"\nI4:"<<endl;
    for (  i = 0; i < Ivec.size(); i++)
    {
        dispItem( lang,  Ivec[i]);
    }
    //I5
    Ivec.clear();
    item.setDot(3);
    item.setRuleInd(1);
    item.ComputerEClosure(lang,Ivec);
    cout<<"\nI5:"<<endl;
    for (  i = 0; i < Ivec.size(); i++)
    {
        dispItem( lang,  Ivec[i]);
    }
}
void test7()
{
        CLanguage lang;

    //定义一个符号集合V
    CSymbol accept(1,"accept");
    CSymbol command(2,"command");//v1
    CSymbol exp(3, "exp");  //v2
    CSymbol Add(4,"+"); //v3 
    CSymbol term(5,"term");//v4
    CSymbol Sub (6,"-");//v5
    CSymbol Mul (7,"*");//v6
    CSymbol factor (8,"factor");//v7
    CSymbol NUMBER (9,"NUMBER");//v8
    CSymbol LP (10,"(");//v9
    CSymbol RP (11,")");//v10
    //一.获得CLanguage之vector<CSymbol>
    lang.InsertSymbol(accept);
    lang.InsertSymbol(command);         //1
    lang.InsertSymbol(exp);
    lang.InsertSymbol(Add);         //3 
    lang.InsertSymbol(term);         //4
    lang.InsertSymbol(Sub);         //5
    lang.InsertSymbol(Mul);        
    lang.InsertSymbol(factor);
    lang.InsertSymbol(NUMBER);  //3 
    lang.InsertSymbol(LP);         //4
    lang.InsertSymbol(RP);         
    showsym(lang.symbol);         //显示之

    //二.获得CLanguage之vector<Rules>
    //1.获得rightpart之Vector<CSymbol*>
#define vlang(x) (&lang.symbol[x])
    //rule1
    vector<CSymbol*> rpart1;
    rpart1.push_back(vlang( 2 ));

    //2。获得leftpart--取得rules
    CRules r1(vlang(1),rpart1);
    //3.插入vector<Rules> 
    showrule( r1 );
    //r2
    vector<CSymbol*> rpart2;
    rpart2.push_back(vlang( 3 ));
    CRules r2(vlang(2),rpart2);
    showrule( r2 );

    //r3
    vector<CSymbol*> rpart3;
    rpart3.push_back(vlang(3));
    rpart3.push_back(vlang(4));
    rpart3.push_back(vlang(5));
    CRules r3(vlang(3),rpart3);
    showrule( r3 );

    //r4
    vector<CSymbol*> rpart4;
    rpart4.push_back(vlang(3));
    rpart4.push_back(vlang(6));
    rpart4.push_back(vlang(5));    
    CRules r4(vlang(3),rpart4);
    showrule( r4 );

    //r5
    vector<CSymbol*> rpart5;
    rpart5.push_back(vlang(5));    
    CRules r5(vlang(3),rpart5);
    showrule( r5 );

    //r6
    vector<CSymbol*> rpart6;
    rpart6.push_back(vlang(5));    
    rpart6.push_back(vlang(7));    
    rpart6.push_back(vlang(8));    
    CRules r6(vlang(5),rpart6);
    showrule( r6);

    //r7
    vector<CSymbol*> rpart7;
    rpart7.push_back(vlang(8));    
    CRules r7(vlang(5),rpart7);
    showrule( r7 );

    //r8
    vector<CSymbol*> rpart8;
    rpart8.push_back(vlang(9));     
    CRules r8(vlang(8),rpart8);
    showrule( r8 );

    //r9
    vector<CSymbol*> rpart9;
    rpart9.push_back(vlang(10));  
    rpart9.push_back(vlang(3));  
    rpart9.push_back(vlang(11));  
    CRules r9(vlang(8),rpart9);
    showrule( r9 );
    
    cout<<" \nThe language's symbol is "<<endl;

    showsym( lang.symbol );

    lang.InsertRules(r1);
    lang.InsertRules(r2);
    lang.InsertRules(r3);
    lang.InsertRules(r4);
    lang.InsertRules(r5);
    lang.InsertRules(r6);
    lang.InsertRules(r7);
    lang.InsertRules(r8);
    lang.InsertRules(r9);
    cout << "\nThe languge's rules is"<<endl; 
    for ( int i = 0; i<lang.rules.size(); i++)
       showrule(lang.rules[i]);
#undef vlang 

    //三.获得开始符号
    lang.SetStartSymbol(lang.symbol[1]);
    cout<<"\nlanguage's startsymbol is "<<lang.startsym->name
    	<<endl;
    //language初始化结束

    //计算first集合
    //1.计算所有终结符的first集合
    //lang.FirstAllTerm();
    //2.从rules中计算first集合
    lang.ComputerFirstSet();
    for (  i = 0; i<lang.symbol.size(); i++)
       showFirst(lang.symbol[i],lang);

    //设置一个item 并计算其closure
    //I0
    CItem firstItem(0,0);
    CIset aIset(firstItem);
    vector<CItem> Ivec;
    aIset.ComputerE(aIset.Icore[0],Ivec,lang);
    disp(lang,Ivec);
}
void test6()
{ 
    CLanguage lang;

    //定义一个符号集合V
    CSymbol accept(1,"accept");
    CSymbol command(2,"command");//v1
    CSymbol exp(3, "exp");  //v2
    CSymbol Add(4,"+"); //v3 
    CSymbol term(5,"term");//v4
    CSymbol Sub (6,"-");//v5
    CSymbol Mul (7,"*");//v6
    CSymbol factor (8,"factor");//v7
    CSymbol NUMBER (9,"NUMBER");//v8
    CSymbol LP (10,"(");//v9
    CSymbol RP (11,")");//v10
    //一.获得CLanguage之vector<CSymbol>
    lang.InsertSymbol(accept);
    lang.InsertSymbol(command);         //1
    lang.InsertSymbol(exp);
    lang.InsertSymbol(Add);         //3 
    lang.InsertSymbol(term);         //4
    lang.InsertSymbol(Sub);         //5
    lang.InsertSymbol(Mul);        
    lang.InsertSymbol(factor);
    lang.InsertSymbol(NUMBER);  //3 
    lang.InsertSymbol(LP);         //4
    lang.InsertSymbol(RP);         
    showsym(lang.symbol);         //显示之

    //二.获得CLanguage之vector<Rules>
    //1.获得rightpart之Vector<CSymbol*>
#define vlang(x) (&lang.symbol[x])
    //rule1
    vector<CSymbol*> rpart1;
    rpart1.push_back(vlang( 2 ));

    //2。获得leftpart--取得rules
    CRules r1(vlang(1),rpart1);
    //3.插入vector<Rules> 
    showrule( r1 );
    //r2
    vector<CSymbol*> rpart2;
    rpart2.push_back(vlang( 3 ));
    CRules r2(vlang(2),rpart2);
    showrule( r2 );

    //r3
    vector<CSymbol*> rpart3;
    rpart3.push_back(vlang(3));
    rpart3.push_back(vlang(4));
    rpart3.push_back(vlang(5));
    CRules r3(vlang(3),rpart3);
    showrule( r3 );

    //r4
    vector<CSymbol*> rpart4;
    rpart4.push_back(vlang(3));
    rpart4.push_back(vlang(6));
    rpart4.push_back(vlang(5));    
    CRules r4(vlang(3),rpart4);
    showrule( r4 );

    //r5
    vector<CSymbol*> rpart5;
    rpart5.push_back(vlang(5));    
    CRules r5(vlang(3),rpart5);
    showrule( r5 );

    //r6
    vector<CSymbol*> rpart6;
    rpart6.push_back(vlang(5));    
    rpart6.push_back(vlang(7));    
    rpart6.push_back(vlang(8));    
    CRules r6(vlang(5),rpart6);
    showrule( r6);

    //r7
    vector<CSymbol*> rpart7;
    rpart7.push_back(vlang(8));    
    CRules r7(vlang(5),rpart7);
    showrule( r7 );

    //r8
    vector<CSymbol*> rpart8;
    rpart8.push_back(vlang(9));     
    CRules r8(vlang(8),rpart8);
    showrule( r8 );

    //r9
    vector<CSymbol*> rpart9;
    rpart9.push_back(vlang(10));  
    rpart9.push_back(vlang(3));  
    rpart9.push_back(vlang(11));  
    CRules r9(vlang(8),rpart9);
    showrule( r9 );
    
    cout<<" \nThe language's symbol is "<<endl;

    showsym( lang.symbol );

    lang.InsertRules(r1);
    lang.InsertRules(r2);
    lang.InsertRules(r3);
    lang.InsertRules(r4);
    lang.InsertRules(r5);
    lang.InsertRules(r6);
    lang.InsertRules(r7);
    lang.InsertRules(r8);
    lang.InsertRules(r9);
    cout << "\nThe languge's rules is"<<endl; 
    for ( int i = 0; i<lang.rules.size(); i++)
       showrule(lang.rules[i]);
#undef vlang 

    //三.获得开始符号
    lang.SetStartSymbol(lang.symbol[1]);
    cout<<"\nlanguage's startsymbol is "<<lang.startsym->name
    	<<endl;
    //language初始化结束

    //计算first集合
    //1.计算所有终结符的first集合
    //lang.FirstAllTerm();
    //2.从rules中计算first集合
    lang.ComputerFirstSet();
    for (  i = 0; i<lang.symbol.size(); i++)
       showFirst(lang.symbol[i],lang);

    //设置一个item 并计算其closure
    //I0
    CItem firstItem(0,0);
    CIset aIset(firstItem);
    aIset.ComputeAllCore(lang,aIset.Icore);
    dispIcore(aIset,lang);
}

void test5()
{ 
    CLanguage lang;

    //定义一个符号集合V
    CSymbol A1(1,"A1");//v1
    CSymbol A(2, "A");  //v2
    CSymbol LP(3,"("); //v3 
    CSymbol RP(4,")");//v4
    CSymbol a  (5,"a");//v5
    //一.获得CLanguage之vector<CSymbol>
    lang.InsertSymbol(A1);         //1
    lang.InsertSymbol(A);
    lang.InsertSymbol(LP);         //3 
    lang.InsertSymbol(RP);         //4
    lang.InsertSymbol(a);         //5
    showsym(lang.symbol);                //显示之

    //二.获得CLanguage之vector<Rules>
    //1.获得rightpart之Vector<CSymbol*>
#define vlang(x) (&lang.symbol[x])
    //rule1
    vector<CSymbol*> rpart1;
    rpart1.push_back(vlang( 2 ));

    //2。获得leftpart--取得rules
    CRules r1(vlang(1),rpart1);
    //3.插入vector<Rules> 
    showrule( r1 );

    //r2
    vector<CSymbol*> rpart2;
    rpart2.push_back(vlang(3));
    rpart2.push_back(vlang(2));
    rpart2.push_back(vlang(4));
    CRules r2(vlang(2),rpart2);
    showrule( r2 );

    //r3
    vector<CSymbol*> rpart3;
    rpart3.push_back(vlang(5));
    CRules r3(vlang(2),rpart3);
    showrule( r3 );



    cout<<" \nThe language's symbol is "<<endl;

    showsym( lang.symbol );

    lang.InsertRules(r1);
    lang.InsertRules(r2);
    lang.InsertRules(r3);

    cout << "\nThe languge's rules is"<<endl; 
    for ( int i = 0; i<lang.rules.size(); i++)
       showrule(lang.rules[i]);
#undef vlang 

    //三.获得开始符号
    lang.SetStartSymbol(lang.symbol[1]);
    cout<<"\nlanguage's startsymbol is "<<lang.startsym->name
    	<<endl;
    //language初始化结束

    //计算first集合
    //1.计算所有终结符的first集合
    //lang.FirstAllTerm();
    //2.从rules中计算first集合
    lang.ComputerFirstSet();
    for (  i = 0; i<lang.symbol.size(); i++)
       showFirst(lang.symbol[i],lang);

    //设置一个item 并计算其closure
    //I0
    CItem firstItem(0,0);
    CIset aIset(firstItem);
    aIset.ComputeAllCore(lang,aIset.Icore);
    dispIcore(aIset,lang);
}
void test4()
{
	    CLanguage lang;

	//定义一个符号集合V
    CSymbol A1(1,"A1");//v1
    CSymbol A(2, "A");  //v2
    CSymbol LP(3,"("); //v3 
    CSymbol RP(4,")");//v4
    CSymbol a  (5,"a");//v5
    //一.获得CLanguage之vector<CSymbol>
    lang.InsertSymbol(A1);         //1
    lang.InsertSymbol(A);
    lang.InsertSymbol(LP);         //3 
    lang.InsertSymbol(RP);         //4
    lang.InsertSymbol(a);         //5
    showsym(lang.symbol);                 //显示之

    //二.获得CLanguage之vector<Rules>
    //1.获得rightpart之Vector<CSymbol*>
#define vlang(x) (&lang.symbol[x])
   //rule1
    vector<CSymbol*> rpart1;
    rpart1.push_back(vlang( 2 ));

    //2。获得leftpart--取得rules
    CRules r1(vlang(1),rpart1);
    //3.插入vector<Rules> 
    showrule( r1 );

    //r2
    vector<CSymbol*> rpart2;
    rpart2.push_back(vlang(3));
    rpart2.push_back(vlang(2));
    rpart2.push_back(vlang(4));
    CRules r2(vlang(2),rpart2);
    showrule( r2 );

    //r3
    vector<CSymbol*> rpart3;
    rpart3.push_back(vlang(5));
    CRules r3(vlang(2),rpart3);
    showrule( r3 );


    
    cout<<" \nThe language's symbol is "<<endl;
   
    showsym( lang.symbol );

    lang.InsertRules(r1);
    lang.InsertRules(r2);
    lang.InsertRules(r3);
    
    cout << "\nThe languge's rules is"<<endl; 
    for ( int i = 0; i<lang.rules.size(); i++)
       showrule(lang.rules[i]);
#undef vlang 

	//三.获得开始符号
	lang.SetStartSymbol(lang.symbol[1]);
	cout<<"\nlanguage's startsymbol is "<<lang.startsym->name
		<<endl;
	//language初始化结束

	//计算first集合
	//1.计算所有终结符的first集合
	//lang.FirstAllTerm();
    //2.从rules中计算first集合
    lang.ComputerFirstSet();
    for (  i = 0; i<lang.symbol.size(); i++)
       showFirst(lang.symbol[i],lang);
    
    //设置一个item 并计算其closure
    CItem item;
    vector < CItem >  Ivec;

    //I0
    item.setDot( 0 );
    item.setRuleInd( 0 );
    item.ComputerEClosure(lang ,Ivec);
    //其中Ivec[0] 是核心项目，切不可直接使用item 计算
    Ivec[0].precs.insert(0);//插入"$"
    Ivec[0].ComputerEPrecs(lang,  Ivec);
    cout<<"\nI0:"<<endl;
    for (  i = 0; i < Ivec.size(); i++)
    {
        dispItem( lang,  Ivec[i]);
    }

    //I1
    Ivec[0].Go(item,lang);
    Ivec.clear();
    item.ComputerEClosure(lang,Ivec);
    Ivec[0].ComputerEPrecs(lang,  Ivec);
    cout<<"\nI1:"<<endl;
    for (  i = 0; i < Ivec.size(); i++)
    {
        dispItem( lang,  Ivec[i]);
    }
    //I2
    Ivec.clear();
    item.setDot(1);
    item.setRuleInd(2);
    item.ComputerEClosure(lang,Ivec);
    Ivec[0].precs.insert(0);//插入"$"
    Ivec[0].precs.insert(4);//插入)
    Ivec[0].ComputerEPrecs(lang,  Ivec);
    cout<<"\nI2:"<<endl;
    for (  i = 0; i < Ivec.size(); i++)
    {
        dispItem( lang,  Ivec[i]);
    }
    //I3
    Ivec.clear();
    item.setDot(1);
    item.setRuleInd(1);
    item.ComputerEClosure(lang,Ivec);
    Ivec[0].precs.insert(0);//插入"$"
    Ivec[0].precs.insert(4);//插入)
    Ivec[0].ComputerEPrecs(lang,  Ivec);
    cout<<"\nI3:"<<endl;
    for (  i = 0; i < Ivec.size(); i++)
    {
        dispItem( lang,  Ivec[i]);
    }
    //I4
    Ivec.clear();
    item.setDot(2);
    item.setRuleInd(1);
    item.ComputerEClosure(lang,Ivec);
    Ivec[0].precs.insert(0);//插入"$"
    Ivec[0].precs.insert(4);//插入)
    Ivec[0].ComputerEPrecs(lang,  Ivec);
    cout<<"\nI4:"<<endl;
    for (  i = 0; i < Ivec.size(); i++)
    {
        dispItem( lang,  Ivec[i]);
    }
    //I5
    Ivec.clear();
    item.setDot(3);
    item.setRuleInd(1);
    item.ComputerEClosure(lang,Ivec);
    Ivec[0].precs.insert(0);//插入"$"
    Ivec[0].precs.insert(4);//插入)
    Ivec[0].ComputerEPrecs(lang,  Ivec);
    cout<<"\nI5:"<<endl;
    for (  i = 0; i < Ivec.size(); i++)
    {
        dispItem( lang,  Ivec[i]);
    }
}
#if 0
void main() {
  SET_INT s1;
  SET_INT s2;
  SET_INT::iterator i;
  cout << "s1.insert(5)" << endl;
  s1.insert(5);
  cout << "s1.insert(10)" << endl;
  s1.insert(10);
  cout << "s1.insert(15)" << endl;
  s1.insert(15);
  cout << "s2.insert(2)" << endl;
  s2.insert(2);
  cout<<"first"<<endl;
 if ( (s2.insert(4)).second)
  cout << "s2.insert(4)" << endl;
 else 
 {
     cout<<"4 is exist."<<endl;
 }
   cout<<"second"<<endl;

 if ( (s2.insert(4)).second)
  cout << "s2.insert(4)" << endl;
 else 
 {
     cout<<"4 is exist."<<endl;
 }

    for (i=s2.begin();i!=s2.end();i++)
  cout<< "s2 has "<<*i<<endl;

  cout << "swap(s1,s2)" << endl;
  swap(s1,s2);

  // Displays: 2,4
  for (i=s1.begin();i!=s1.end();i++)
     cout << "s1 has " << *i << " in its set." << endl;

  // Displays: 5,10,15
  for (i=s2.begin();i!=s2.end();i++)
     cout << "s2 has " << *i << " in its set." << endl;

  cout << "s1.swap(s2)" << endl;
  s1.swap(s2);

  // Displays: 5,10,15
  for (i=s1.begin();i!=s1.end();i++)
     cout << "s1 has " << *i << " in its set." << endl;

  // Displays: 2,4
  for (i=s2.begin();i!=s2.end();i++)
     cout << "s2 has " << *i << " in its set." << endl;
  cout<<"It has ";
  cout<< SetUnion(s1,s2) 
      <<"changes"<<endl;
  for (i=s1.begin();i!=s1.end();i++)
     cout << "s1 has " << *i << " in its set." << endl;
s2.insert(6);
    cout<<"It has ";
  cout<< SetUnion(s1,s2) 
      <<"changes"<<endl;
  for (i=s1.begin();i!=s1.end();i++)
     cout << "s1 has " << *i << " in its set." << endl;

}
void main()
{
  CFirstSet s1(5);
  CFirstSet s2;
  SET_INT::iterator i;
  cout << "s1.insert(5)" << endl;
   for (i=s1.set.begin();i!=s1.set.end();i++)
  cout<< "s1 has "<<*i<<endl;

   s2.insert(4);
   for (i=s2.set.begin();i!=s2.set.end();i++)
  cout<< "s2 has "<<*i<<endl;

  cout<<"s1.SetUnion(s1.set,s2.set)"<<endl;
  s1.SetUnion(s1.set,s2.set);
  
  for (i=s1.set.begin();i!=s1.set.end();i++)
  cout<< "s1 has "<<*i<<endl;
  string a = "a";
  CSymbol sy(1,a,false,false);
}
void main()
{
    CLanguage lang;

	//定义一个符号集合V
    CSymbol stmt_sequence(1,"stmt_sequence");
    CSymbol stmt(2,"stmt");
    CSymbol stmt_seq(3,"stmt_seq");
    CSymbol semicolon(4,";");
    CSymbol s(5,"s");

    //一.获得CLanguage之vector<CSymbol>
    vector<CSymbol> vSym;
    lang.InsertSymbol(stmt_sequence);//1
    lang.InsertSymbol(stmt);         //2 
    lang.InsertSymbol(stmt_seq);     //3
    lang.InsertSymbol(semicolon);    //4
    lang.InsertSymbol(s);            //5
    showsym(lang.symbol);                //显示之

    //二.获得CLanguage之vector<Rules>
    //1.获得rightpart之Vector<CSymbol*>
#define vlang(x) (&lang.symbol[x])
    //rule1
    vector<CSymbol*> rpart1;
    rpart1.push_back(vlang( 2 ));
       cout<<vlang(2)<<" part"<<endl;
       cout<<rpart1[0]<<endl;
    rpart1.push_back(vlang( 3 ));
    //2。获得leftpart--取得rules
    CRules r1(vlang(1),rpart1);
    //3.插入vector<Rules>
    showrule( r1 );

    //r2
    vector<CSymbol*> rpart2;
    rpart2.push_back(vlang(4));
    rpart2.push_back(vlang(1));
    CRules r2(vlang(3),rpart2);
    showrule( r2 );

    //r3
    vector<CSymbol*> rpart3;
    CRules r3(vlang(3),rpart3);
    showrule( r3 );

    //r4
    vector<CSymbol*> rpart4;
    rpart4.push_back(vlang(5));
    CRules r4(vlang(2),rpart4);
    showrule(r4);

    cout<<" \nThe language's symbol is "<<endl;
   
    showsym( lang.symbol );
    lang.InsertRules(r1);
    lang.InsertRules(r2);
    lang.InsertRules(r3);
    lang.InsertRules(r4);
    cout << "\nThe languge's rules is"<<endl; 
    for ( int i = 0; i<lang.rules.size(); i++)
       showrule(lang.rules[i]);
#undef vlang 

	//三.获得开始符号
	lang.SetStartSymbol(lang.symbol[1]);
	cout<<"\nlanguage's startsymbol is "<<lang.startsym->name
		<<endl;
	//language初始化结束

	//计算first集合
	//1.计算所有终结符的first集合
	//lang.FirstAllTerm();
    lang.ComputerFirstSet();
    for (  i = 0; i<lang.symbol.size(); i++)
       showFirst(lang.symbol[i],lang);
    //2.从rules中计算first集合

}
#endif

void main()
{ 
#if 0
    cout<<"\n----Test1-------- "<<endl;
    test1();
    cout<<"\n------Test2------ "<<endl;
    test2();
    cout<<"\n------Test21----- "<<endl;
    test21();
    cout<<"\n---------+Test3+----"<<endl;
    test3();
    cout<<"\n*******===Test4====***"<<endl;
    test4();
    cout<<"\n*******===Test5====***"<<endl;

    test7();
    test5();
    test6();
    test8();
    test9();
    test10();
#endif
    test11();
}
