
#include <cstdlib>

//#include <functional>
#include <iostream>

#include "..\vs_command2.hpp"


//using namespace std::cout;
using std::cout;
using namespace vslib;


int foo(bool b, float f)
{
  printf("int foo(%i, %f)\n", b, f);
  return (int)f * (int)b;
}

void bar()
{
  cout << "void bar()" << endl;
}


int main()
{
  /*function<int (bool, float)> f1 = foo;
  int k = f1(true, 6.98);
  cout << k << "\n---\n" << endl;//*/

  /*function<int (bool, float)> f1 {foo};
  function<void ()> f2 {bar};
  int k = f1(true, 6.98);
  cout << k << endl;
  f2();
  cout << "\n---\n" << endl;//*/

  function2<int (bool, float)> f3 {foo};
  function2<void ()> f4, f5;
  f4 = bar;
  f5 = f4;
  f4();
  f5();
  int k2 = f3(true, 17.01);
  cout << k2 << "\n---\n" << endl;

  DoSomething<void, int, bool, float>(5, 8.4, 7.9);
  //print(1, true, 8.4);

  system("pause");
  return 0;
}
