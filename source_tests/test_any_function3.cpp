
#include <cstdlib>

#include <iostream>
#include <functional>
#include <memory>
#include <vector>

#include "..\vs_any_function.hpp"


using namespace std;
using namespace vslib;


class Funct1 {
public:
  int operator()(int n) {
    printf("Funct1(%i)\n", n);
    return n;
  }
};

class Cl1 {
public:
  int DoSomething1(int n) {
    printf("Cl1.DoSomething(%i)\n", n);
    return _n;
  }
private:
  int _n{71};
};

int GlobalFunc1(int n)
{
  printf("GlobalFunc1(%i)\n", n);
  return n + 100000;
}

using FuncPtr2 = AnyFunctionPtr<int (int)>;

void DoFunc(const FuncPtr2& f)
{
  if (f)
    cout << f(65) << "\n\n";//endl;
  else
    cout << "f == nullptr\n\n";
}


int main()
{
  printf(
    "sizeof(AnyFunction) = %lli\n"
    "sizeof(AnyFunctionPtr) = %lli\n"
    "sizeof(Method) = %lli\n"
    "sizeof(Function) = %lli\n"
    "sizeof(Functor) = %lli\n"
    "sizeof(std::function) = %lli\n\n",
    sizeof(AnyFunction<void ()>),
    sizeof(AnyFunctionPtr<void ()>),
    sizeof(Method<Cl1, void (void)>),
    sizeof(Function<void>),//sizeof(Function<void (void)>),//sizeof(Func<void ()>::Type),
    sizeof(Functor<void>),//sizeof(Functor<void ()>),
    sizeof(std::function<void ()>)
  );//*/

  Cl1 o1;

  FuncPtr2 f1;
  //f1 = {&o1, o1.DoSomething1};// It won't work..
  f1.Assign(&o1, o1.DoSomething1);
  //f1 = make_pair(&o1, o1.DoSomething1);
  f1 = MakeMethod(&o1, o1.DoSomething1);
  DoFunc(f1);
  f1 = GlobalFunc1;
  DoFunc(f1);
  //f1 = [](int n) -> int {printf("Lambda1(%i)\n", n); return n + 100;};
  f1 = (FuncPtr2::TheFunctionPtr)[](int n) -> int {printf("Lambda1(%i)\n", n); return n + 100;};
  DoFunc(f1);
  f1 = nullptr;
  DoFunc(f1);
  f1 = Funct1();
  DoFunc(f1);


  /*int (*f3)(int) = GlobalFunc1;
  try {
    cout << "try:\n";
    f3 = nullptr;
    f3(43);
  } catch (...) {
    cout << "Catch exception!\n\n";
  }//*/

  /*try {
    //f1 = MakeMethod(&o1, o1.DoSomething1);
    f1 = MakeMethod<Cl1>(nullptr, o1.DoSomething1);
    //f1 = vslib::MakeMethod<Cl1, int, int>(nullptr, nullptr);
    f1(555);
  } catch (...) {
    cout << "Nope..\n";
  }//*/

  int k = 777;
  //f1 = nullptr;
  f1.Execute(651, &k);
  cout << k << "\n\n";//endl;//*/

  /*using fptr = int (*)(int);
  auto l1 = [](int n) -> int {printf("Lambda2(%i)\n", n); return n + 100;};
  printf("\nsizeof(lambda) = %i\n", sizeof(l1));
  printf("sizeof(int (*)(int)) = %i\n\n", sizeof(int(*)(int)));
  fptr f2 = l1;
  fptr f6 = [](int){return 0;};
  cout << f2(909) << "\n\n";

  Funct1 f3;
  //fptr f4 = &f3;
  function<int (int)> f5 = Funct1();
  cout << f5(-70) << "\n\n";//*/

  system("pause");
  return 0;
}
