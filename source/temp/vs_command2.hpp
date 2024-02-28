/*
  (c) Vitaly Smirnov [VSdev]
  mrmaybelately@gmail.com
  2023
*/

#ifndef VS_COMMAND_HPP_INCLUDED
#define VS_COMMAND_HPP_INCLUDED


#include <iostream>
//#include <functional>
#include <memory>


using namespace std;



namespace vslib
{

  void print()
  {
    cout << endl;
  }

  template <typename T>
  void print(const T& t)
  {
    cout << t << endl;
  }

  template <typename First, typename... Rest>
  void print(const First& first, const Rest&... rest)
  {
      cout << first << ", ";
      print(rest...); // recursive call using pack expansion syntax
  }


  /*template <typename ResType (typename... Args)>
  ResType DoSomething2(Args... args)
  {
    constexpr auto numargs{sizeof...(Args)};
    printf("args count: %i\n", numargs);//sizeof...(Args));
  }//*/

  template <typename ResType, typename... Args>
  ResType DoSomething(Args... args)
  {
    constexpr auto numargs{sizeof...(Args)};
    printf("args count: %i\n", numargs);//sizeof...(Args));
  }

  /*template <typename T, typename... Arguments>
    SimpleUniquePointer<T> make_simple_unique(Arguments... arguments) {
    return SimpleUniquePointer<T>{ new T{ arguments... } };
  }//*/


  template <typename UnusedType>
  class function;

  template <typename ReturnType, typename ... ArgumentTypes>
  class function <ReturnType (ArgumentTypes ...)> {
  public:
    function() : mInvoker() {}
    template <typename FunctionT>
    function(FunctionT f) : mInvoker(new free_function_holder<FunctionT>(f)) {}

    ReturnType operator ()(ArgumentTypes ... args) {
      return mInvoker->invoke(args ...);
    }
  private:
    class function_holder_base {
    public:
      function_holder_base() {}
      virtual ~function_holder_base() {}
      virtual ReturnType invoke(ArgumentTypes ... args) = 0;
    };
    typedef std::auto_ptr<function_holder_base> invoker_t;

    template <typename FunctionT>
    class free_function_holder : public function_holder_base {
    public:
      free_function_holder(FunctionT func) : function_holder_base(), mFunction(func) {}

      virtual ReturnType invoke(ArgumentTypes ... args) {
        return mFunction(args ...);
      }
    private:
      FunctionT mFunction;
    };
    invoker_t mInvoker;
  };

  /// ///

  template <typename UnusedType>
  class function2;

  //template <typename ReturnType, typename ... ArgumentTypes>
  //class function2 <ReturnType (ArgumentTypes ...)> {

  //template <>
  //class function2 <void ()>;

  template <typename ReturnType, typename ... ArgumentTypes>
  class function2 <ReturnType (ArgumentTypes ...)> {
  public:
    typedef ReturnType (*func_ptr)(ArgumentTypes ...);
    //using func_ptr = ReturnType(*)(ArgumentTypes ...);
  public:
    function2() = default;
    function2(func_ptr f): _func(f) {}

    //void operator=(const function2 <ReturnType (ArgumentTypes ...)> &) = delete;
    //void operator=(const function2 <ReturnType (ArgumentTypes ...)> &) = default;
    void operator=(func_ptr f) {_func = f;}

    ReturnType operator()(ArgumentTypes ... args) {
      if (_func)
        return (*_func)(args ...);
    }
  private:
    func_ptr _func{nullptr};
  };

}


#endif // VS_COMMAND_HPP_INCLUDED
