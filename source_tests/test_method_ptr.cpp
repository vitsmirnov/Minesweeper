
#include <cstdlib>

#include <functional>
#include <iostream>
#include <memory>

#include "..\vs_method_ptr.hpp"


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


//using fptr1 = int (Cl1::*)(int);

void func1(Cl1* c, int (Cl1::*f)(int))
//void func1(Cl1* c, fptr1 f)
{
  if (c && f)
    (c->*f)(5);
}


int main()
{
  using MethodPtr1 = method_ptr<Cl1, int (int)>;
  Cl1 o1;
  /*MethodPtr1 mp1(&o1, nullptr);//Cl1::DoSomething1);
  int k = 63;
  k = mp1(5);
  cout << k << endl;

  //func1(&o1, Cl1::DoSomething1);

  printf("\n\n-------\n");
  //auto func2 = make_method(&o1, Cl1::DoSomething1);
  using method = abstract_method_ptr<int (int)>;
  using pmethod = unique_ptr<method>;
  pmethod func2 {make_method(&o1, Cl1::DoSomething1)};
  cout << (*func2)(197) << endl;//*/

  printf("\n\n-------\n");
  //auto pm = make_method(&o1, Cl1::DoSomething1);
  MethodHolder<int (int)> m1 {make_method(&o1, Cl1::DoSomething1)};//{pm};
  m1 = make_method(&o1, Cl1::DoSomething1);
  auto m2 = make_m_holder(&o1, Cl1::DoSomething1);
  //m1._ptr.reset(make_method(&o1, Cl1::DoSomething1));
  m1(111);
  m2(333);
  cout << m2(1222) << endl;

  std::function<int (Cl1*, int)> m3;// {Cl1::DoSomething1};
  m3 = Cl1::DoSomething1;
  if (m3 != nullptr)
    m3(&o1, 4444);
  else
    cout << "m3 = nullptr\n";

  system("pause");
  return 0;
}
