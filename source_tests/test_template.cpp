#include <cstdlib>
#include <cstdio>

#include <iostream>


using namespace std;


int F1(int n)
{
  printf("F1(%i)\n", n);
  return n + 1000;
}


template<typename _Res, typename _Arg>
using FPtr = _Res (*)(_Arg);

using FPtrii = FPtr<int, int>;

template<typename _Res, typename ... _Args>
using FPtr2 = _Res (*)(_Args ...);


template<typename _T>
void DoFunc(const FPtrii& f, _T arg)
{
  if (f)
    cout << f(arg) << "\n\n";
  else
    cout << "*func == nullptr\n\n";
}


int main()
{
  FPtrii f1;
  f1 = F1;
  DoFunc(f1, 7);

  FPtr2<int, int> f2 = F1;
  DoFunc(f2, -3);


  system("pause");
  return 0;
}
