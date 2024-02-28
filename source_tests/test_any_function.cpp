
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


int main()
{
  printf("sizeof(AnyFunction) = %i\nsizeof(AnyFunctionPtr) = %i\nsizeof(Method) = %i\n",
    sizeof(AnyFunction<void ()>), sizeof(AnyFunctionPtr<void ()>), sizeof(Method<Cl1, void ()>));
  printf("sizeof(std::function) = %i\n", sizeof(std::function<void ()>));

  Cl1 o1;
  using FuncPtr = AnyFunctionPtr<int (int)>;
  using Meth = Method<Cl1, int (int)>;
  FuncPtr f1 {new Method<Cl1, int (int)>(&o1, Cl1::DoSomething1)};
  FuncPtr f2 {new Meth(&o1, Cl1::DoSomething1)};
  FuncPtr f3 {MakeMethod(&o1, Cl1::DoSomething1)};
  auto f4 = MakeAnyFuncPtr(&o1, Cl1::DoSomething1);
  cout << f1(6) << endl;
  cout << f2(67) << endl;
  cout << f3(678) << endl;
  cout << f4(6789) << endl;
  Meth::RootClass* m1 = MakeMethod(&o1, Cl1::DoSomething1);
  (*m1)(-10);
  auto u1 = make_unique<Meth>(&o1, Cl1::DoSomething1);
  (*u1)(7);

  printf("\n-------\n");
  vector<unique_ptr<AnyFunction<int (int)>>> v1;
  auto f7 = MakeAnyFuncPtr2(&o1, Cl1::DoSomething1);
  auto f8 = MakeAnyFuncPtr2(GlobalFunc1);
  function<int (int)> l1 = [](int n){printf("Do lambda\n");return int(n - 1000);};
  //auto f9 = MakeAnyFuncPtr2([](int n){printf("Do lambda\n");return int(n - 1000);});
  auto f9 = MakeAnyFuncPtr2(l1);
  auto f10 = MakeAnyFuncPtr2(
    function<int (int)>([](int n){printf("Do lambda2\n");return int(n - 10000);})
  );
  cout << (*f7)(123) << endl;
  cout << (*f8)(456) << endl;
  cout << (*f9)(789) << endl;
  cout << (*f10)(789) << endl;
  cout << l1(789) << endl;

  system("pause");
  return 0;
}
