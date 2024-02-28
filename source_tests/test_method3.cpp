#include <cstdlib>
#include <cstdio>

#include <iostream>
#include <functional>

//#define METHOD_NULL_CREATION
#include "..\vs_method_ptr.hpp"


using namespace std;
using namespace vslib;


class SomeCl1 {
public:
  int DoSomething1(int n) {
    printf("SomeCl1.DoSomething1(%i)\n", n);
    return n + _n;
  }
  void DoSomething2(int n) {
    printf("SomeCl1.DoSomething2(%i)\n", n + _n);
    //return n - 1000;
  }
  int DoSomething3(int n) {
    printf("SomeCl1.DoSomething3(%i)\n", n);
    return n - _n;
  }
private:
  int _n{888};
};


int Func1(int n)
{
  printf("Func1(%i)\n", n);
  return n + 1000;
}


template<typename _ResType, typename ... _ArgTypes>
void CallFunc(const function<_ResType (_ArgTypes ...)> & f, _ArgTypes ... args)
{
  if (f)
    cout << f(args ...) << "\n\n";
  else
    cout << "CallFunc: func == nullptr\n\n";
}


template<class ... _Parents>
class TestClass1: public _Parents... {};

template<class _Parent = void>
class TestClass2: public _Parent {};

void TestClassTemplateInheritance()
{
  TestClass1<SomeCl1> cl2;
  TestClass1<> cl3;
  //cl3.DoSomething1(6); //Error!
  cout << cl2.DoSomething3(7) << "\n";

  TestClass2<SomeCl1> cl4;
  //TestClass2<void> cl5;//Nope..
  cout << cl4.DoSomething3(-100) << "\n";
  system("pause");
}

int main()
{
  //TestClassTemplateInheritance();return 0;
  /*printf(
    "sizeof(Method) = %lli\n"
    "sizeof(Method2) = %lli\n"
    "sizeof(std::function) = %lli\n\n",
    sizeof(Method<SomeCl1, void (void)>),
    sizeof(Method2<SomeCl1, void (void)>),
    sizeof(std::function<void ()>)
  );//*/

  using FuncType1 = function<int(int)>;
  using Cl1Method = Method<SomeCl1, int (int)>;
  SomeCl1 cl1;
  FuncType1 f1;

  f1 = Cl1Method(&cl1, cl1.DoSomething1);
  function<int (int)> f2 = Method<SomeCl1, int (int)>(&cl1, cl1.DoSomething3);
  //cout << f2(-111) << " <--\n\n";
  //f2(-111);
  //CallFunc(f1, 76);

  FuncType1 f3;
  //f3 = Method<SomeCl1, int (int)>(&cl1, cl1.DoSomething1);
  //f3 = MakeMethod<SomeCl1>(nullptr, cl1.DoSomething3);
  f3 = MakeMethod(&cl1, cl1.DoSomething3);
  //f3 = nullptr;
  CallFunc(f3, -100);
  //cout << (f1 == f2) << "\n";
  //cout << sizeof(Cl1Method) << "\n";

  FuncType1 m1 = MakeMethod(&cl1, cl1.DoSomething1);
  FuncType1 m2 = MakeMethod(&cl1, cl1.DoSomething1);
  FuncType1 m3 = Cl1Method(&cl1, cl1.DoSomething1);
  try {
    auto m4 = MakeMethod((SomeCl1*)nullptr, cl1.DoSomething1);
    m4(6);
  } catch (const char* msg) {
    printf("Error: %s\n", msg);
  }
  //m1 = nullptr;
  //printf("\n-> m1 %s m2 <-\n", m1 == m2 ? "=" : "<>");
  CallFunc(m1, 1);
  CallFunc(m2, 2);
  CallFunc(m3, 3);


  system("pause");
  return 0;
}
