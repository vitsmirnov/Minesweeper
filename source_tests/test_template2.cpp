#include <cstdlib>
#include <cstdio>

#include <iostream>

#include <string>
#include <functional>


using namespace std;


template<class _OwnerClass>
struct MethodData {
public:
  //using MethodPtr = _ResType (_OwnerClass::*)(_ArgTypes ...);
  using MethodPtr = int (_OwnerClass::*)(int);

  MethodData(_OwnerClass* _owner, MethodPtr _method):
    owner{_owner}, method{_method} {};

  _OwnerClass* owner{nullptr};
  MethodPtr method{nullptr};
};

//MethodData() -> MethodData<>;


class Cl1 {
public:
  /*template<class _OwnerClass>
  using MethodData2 = //*/

  template<class _OwnerClass>
  void operator=(const MethodData<_OwnerClass>& method_data) {
    /*_ptr.reset(new Method<_OwnerClass, _ResType (_ArgTypes ...)>(
      method_data.owner, method_data.method)
    );//*/
    printf("Ok, it works.\n");
  }

  int Print(int n) {
    printf("Cl1.Print(%i)\n", n);
    return n + 1000;
  }
private:
  int _field1{17};
  char _field2{'K'};
};

int Func1()
{
  printf("Func1()\n");
  return 98;
}


int main()
{
  Cl1 cl1;
  MethodData md{&cl1, Cl1::Print};
  //cl1 = {&cl1, Cl1::Print};
  cl1.operator=<Cl1>({&cl1, Cl1::Print});
  //cl1.operator=({&cl1, Cl1::Print});

  function<int(void)> f1;
  f1 = Func1;
  f1();

  system("pause");
  return 0;
}
