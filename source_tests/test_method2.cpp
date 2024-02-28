#include <cstdlib>
#include <cstdio>

#include <iostream>
#include <functional>


using namespace std;


template<typename ...>
class Method;

template<class _OwnerClass, typename _ResType, typename ... _ArgTypes>
class Method<_OwnerClass, _ResType (_ArgTypes ...)> {
public:
  using OwnerClass = _OwnerClass;
  using MethodPtr = _ResType (_OwnerClass::*)(_ArgTypes ...);
public:
  Method() = default;
  Method(_OwnerClass* owner, MethodPtr method):
    _owner{owner}, _method{method} {}

  void Assign(_OwnerClass* owner, MethodPtr method)
    {_owner = owner; _method = method;}

  _ResType operator()(_ArgTypes ... args) const
    {return (_owner->*_method)(args ...);}
private:
  _OwnerClass* _owner{nullptr};
  MethodPtr _method{nullptr};
};


template<class _OwnerClass, typename _ResType, typename ... _ArgTypes>
auto MakeMethod(_OwnerClass* owner, _ResType (_OwnerClass::*method)(_ArgTypes ...))
{
  return Method<_OwnerClass, _ResType (_ArgTypes ...)>(owner, method);
}


class SomeCl1 {
public:
  int DoSomething1(int n) {
    printf("SomeCl1.DoSomething1(%i)\n", n);
    return n + 1000;
  }
  void DoSomething2(int n) {
    printf("SomeCl1.DoSomething2(%i)\n", n);
    //return n - 1000;
  }
  int DoSomething3(int n) {
    printf("SomeCl1.DoSomething3(%i)\n", n);
    return n - 1000;
  }
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

  using FuncType1 = function<int(int)>;
  using Cl1Method = Method<SomeCl1, int (int)>;
  SomeCl1 cl1;
  FuncType1 f1;
  f1 = Cl1Method(&cl1, cl1.DoSomething1);
  function<int (int)> f2 = Method<SomeCl1, int (int)>(&cl1, cl1.DoSomething3);
  //cout << f2(-111) << " <--\n\n";
  f2(-111);
  CallFunc(f1, 76);
  FuncType1 f3;
  f3 = Method<SomeCl1, int (int)>(&cl1, cl1.DoSomething1);
  f3 = MakeMethod(&cl1, cl1.DoSomething3);
  //f3 = nullptr;
  CallFunc(f3, -100);
  //cout << (f1 == f2) << "\n";
  //cout << sizeof(Cl1Method) << "\n";

  system("pause");
  return 0;
}
