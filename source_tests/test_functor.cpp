#include <cstdlib>
#include <cstdio>

#include <iostream>


using namespace std;


template<typename _ResType, typename ... _ArgTypes>
using FunctionPtr = _ResType (*)(_ArgTypes ...);//*/

/*template<typename>// ... _ArgTypes>
using FunctionPtr = void (*)(void);

template<typename _ResType, typename ... _ArgTypes>
using FunctionPtr<_ResType (_ArgTypes ...)> = _ResType (*)(_ArgTypes ...);//*/


template<typename ...>
class Functor;

template<typename _ResType, typename ... _ArgTypes>
class Functor<_ResType (_ArgTypes ...)> {
//class Functor<_ResType, _ArgTypes ...> {//??
public:
  //using FuncPtr = _ResType (*)(_ArgTypes ...);
  using FuncPtr = FunctionPtr<_ResType, _ArgTypes ...>;
  //using FuncPtr = FunctionPtr<_ResType (_ArgTypes ...)>;
public:
  explicit Functor(FuncPtr ptr = nullptr): _ptr{ptr}
    {}//{printf("Functor.constructor()\n");}

  void operator=(FuncPtr ptr)
    {_ptr = ptr;}// printf("Functor.operator=()\n");}
  _ResType operator()(_ArgTypes ... args) const
    {return _ptr(args ...);}
private:
  FuncPtr _ptr{nullptr};
};


//template<typename>
//using VoidFunc = Functor<void (void)>;


int Func1(int n)
{
  printf("Func1(%i)\n", n);
  return n + 1000;
}

template<typename _ResType>
_ResType Test2()
{
  return _ResType{};
}


int main()
{
  Functor<int (int)> f1 {Func1};
  f1 = Func1;
  f1 = [](int){printf("Lambda\n"); return (int)0;};
  cout << f1(67) << "\n\n";

  //cout << Test2<Functor<int(int)>*>() << "\n";
  //Test2<Functor<int(int)>*>();

  system("pause");
  return 0;
}
