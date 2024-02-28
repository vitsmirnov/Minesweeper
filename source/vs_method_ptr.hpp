/*
  (c) Vitaly Smirnov [VSdev]
  mrmaybelately@gmail.com
  2023
*/

#ifndef VS_METHOD_PTR_HPP_INCLUDED
#define VS_METHOD_PTR_HPP_INCLUDED


namespace vslib
{


  template<typename ...>
  class Method;  // Rename to MethodPtr?

  template<class _OwnerClass, typename _ResType, typename ... _ArgTypes>
  class Method<_OwnerClass, _ResType (_ArgTypes ...)> {
  public:
    //using OwnerClass = _OwnerClass;
    using MethodPtr = _ResType (_OwnerClass::*)(_ArgTypes ...);
  public:
    Method(_OwnerClass* owner, MethodPtr method):
      _owner{owner}, _method{method}
    {
#ifndef METHOD_NULL_CREATION
      if (!_owner || !_method)
        throw "class Method: trying to create with nullptr!";
#endif // METHOD_SAVE_CREATION
    } //static_assert(owner == nullptr || method == nullptr);

    //void Assign(_OwnerClass* owner, MethodPtr method)
      //{_owner = owner; _method = method;}

    _ResType operator()(_ArgTypes ... args) const
      {return (_owner->*_method)(args ...);}
  private:
    _OwnerClass* _owner{nullptr};
    MethodPtr _method{nullptr};
  };


  template<class _OwnerClass, typename _ResType, typename ... _ArgTypes>
  inline auto MakeMethod(_OwnerClass* owner, _ResType (_OwnerClass::*method)(_ArgTypes ...))
  {
    return Method<_OwnerClass, _ResType (_ArgTypes ...)>(owner, method);
  }

  template<class _OwnerClass, typename _ResType, typename ... _ArgTypes>
  inline auto MakeMethodAsFunc(_OwnerClass* owner, _ResType (_OwnerClass::*method)(_ArgTypes ...))
  {
    return std::function<_ResType (_ArgTypes ...)>{MakeMethod(owner, method)};
  }


} // namespace vslib


#endif // VS_METHOD_PTR_HPP_INCLUDED
