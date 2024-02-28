
#include <cstdlib>

#include <iostream>
#include <functional>
#include <memory>
#include <vector>

#include "..\vs_any_function.hpp"


//using std::cout;
//using std::endl;
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
    "sizeof(AnyFunction) = %i\n"
    "sizeof(AnyFunctionPtr) = %i\n"
    "sizeof(Method) = %i\n"
    "sizeof(Function) = %i\n"
    "sizeof(Functor) = %i\n"
    "sizeof(std::function) = %i\n\n",
    sizeof(AnyFunction<void ()>),
    sizeof(AnyFunctionPtr<void ()>),
    sizeof(Method<Cl1, void (void)>),

    //sizeof(Function<void (void)>),
    sizeof(Function<void>),
    //sizeof(Func<void ()>::Type),

    //sizeof(Functor<void ()>),
    sizeof(Functor<void>),

    sizeof(std::function<void ()>)
  );//*/

  Cl1 o1;

  FuncPtr2 f1;
  //f1 = {&o1, o1.DoSomething1};// It won't work..
  f1.Assign(&o1, o1.DoSomething1);
  //f1 = make_pair(&o1, o1.DoSomething1);
  //f1 = MakeMethod(&o1, o1.DoSomething1);
  DoFunc(f1);
  f1 = GlobalFunc1;
  DoFunc(f1);
  f1 = [](int n) -> int {printf("Lambda1(%i)\n", n); return n + 100;};
  DoFunc(f1);
  f1 = nullptr;
  DoFunc(f1);

  int k = 777;
  f1.Execute(651, &k);
  cout << k << "\n\n";//endl;//*/

  using fptr = int (*)(int);
  auto l1 = [](int n) -> int {printf("Lambda2(%i)\n", n); return n + 100;};
  printf("\nsizeof(lambda) = %i\n", sizeof(l1));
  printf("sizeof(int (*)(int)) = %i\n\n", sizeof(int(*)(int)));
  fptr f2 = l1;
  fptr f6 = [](int){return 0;};
  cout << f2(909) << "\n\n";

  Funct1 f3;
  //fptr f4 = &f3;
  function<int (int)> f5 = Funct1();
  cout << f5(-70) << "\n\n";

  system("pause");
  return 0;
}
