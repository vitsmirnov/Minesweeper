/*
  (c) Vitaly Smirnov [VSdev]
  mrmaybelately@gmail.com
  2023-2024
*/

#include <cstdlib>
#include <cstdio>

#include <functional>


struct P {
  char x, y;

  template<typename _Type>
  constexpr P(_Type _x, _Type _y): x{(char)_x}, y{(char)_y} {}
};

void TestConstexpr1()
{
  constexpr P const_p{18, 9};

  int i{16}, j{9};
  P p1{i, j};
  p1 = {3, 4};
  //p1{5, 7};
  p1 = {i, j};
}


template<typename _FuncType, typename ... _ArgTypes>
void DoFunc(_FuncType f, _ArgTypes ... args)
{
  for (auto i = 0; i < 3; ++i) {
    f(args ...);
  }
}

void F1()
{
  printf("F1 is executed..\n");
}

class Fr1 {
public:
  void operator()() {
    printf("Functor Fr1 is executed..\n");
  }
};

using FPtr = void (*)(int);

void TestLambda1()
{
  int p{115};
  auto l1 = [&](int k){printf("Hello, %i, %i\n", k, p);};
  Fr1 fr1;
  DoFunc(l1, 9);
  DoFunc(F1);
  DoFunc(fr1);
  printf("\nl1 = %lli, F1 = %lli, Fr1 = %lli, FPtr = %lli\n",
    sizeof(l1), sizeof(&F1), sizeof(Fr1), sizeof(FPtr));
}


int main()
{
  TestLambda1();
  TestConstexpr1();

  system("pause");
  return 0;
}

