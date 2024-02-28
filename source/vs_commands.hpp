#ifndef VS_COMMANDS_HPP_INCLUDED
#define VS_COMMANDS_HPP_INCLUDED


//#include <type_traits>
//#include <typeinfo>


#ifdef COMMANDS_INCLUDE_DEFAULT
#include <functional>
#include <map>

//#include "vs_method_ptr.hpp"
#endif // COMMANDS_INCLUDE_DEFAULT


namespace vslib
{

  template<template<typename...> class _FuncPtr,
    template <typename, typename> class _Dictionary,
    typename _KeyType, typename _ResType, typename ... _ArgTypes>
  class Commands;


  // _Dictionary has to have an interface as std::map.
  // _FuncPtr has to have an interface as std::function.

  template<template<typename...> class _FuncPtr,
    template <typename, typename> class _Dictionary,
    typename _KeyType, typename _ResType, typename ... _ArgTypes>
  class Commands<_FuncPtr, _Dictionary, _KeyType, _ResType (_ArgTypes ...)> {
  public:
    using FunctionPtr = _FuncPtr<_ResType (_ArgTypes ...)>;//std::function<_ResType (_ArgTypes ...)>;
    using Dictionary = _Dictionary<_KeyType, FunctionPtr>;//_Dictionary<_KeyType, FunctionPtr>;
    using KeyType = _KeyType;
    //using ResType = _ResType;
  public:
    //explicit Commands(_ObjectType* object = nullptr): _object(object) {}
    //Commands(const Commands& commands) = delete;
    //void operator=(const Commands& commands) = delete;
    //virtual ~Commands() {}

    void SetCommand(const _KeyType& key, const FunctionPtr& f)
      {_functions[key] = f;}
    void DeleteCommand(const _KeyType& key) {_functions.erase(key);}
    /*void DeleteCommand(const FunctionPtr& f) { // This should be checked!
      for (auto &it: _functions)
        if (it.second == f)
          _functions.erase(it.first);
    }//*/
    //void SetCommands(std::initializer_list<const _KeyType&> keys, const FunctionPtr& f) {
    void SetCommands(std::initializer_list<_KeyType> keys, const FunctionPtr& f)
      {for (const auto &key : keys) _functions[key] = f;}//const auto &???
    void Clear() {_functions.clear();}
    //void Unique

    const FunctionPtr* Find(const _KeyType& key) const {
      auto it = _functions.find(key);
      return it != _functions.end() && it->second != nullptr ? &(it->second) : nullptr;
    }
    bool DoCommand(const _KeyType& key, _ArgTypes ... args) const { //rename to Execute?
      auto it = _functions.find(key);
      if (it == _functions.end() || it->second == nullptr)
        return false;
      (it->second)(args ...);
      return true;
    }
    /*_ResType DoCommand(const _KeyType& key, _ArgTypes ... args, bool* success = nullptr) const { //rename to Execute?
      // if _RestType is void, it won't work!
      // #warning It is not good!
      auto it = _functions.find(key);
      bool ok = it != _functions.end() && it->second != nullptr;
      if (success)
        *success = ok;
      return ok ? (it->second)(args ...) : _ResType{};
    }//*/
  private:
    Dictionary _functions;//_Dictionary<_KeyType, FunctionPtr>
  };


#ifdef COMMANDS_INCLUDE_DEFAULT
  template <typename _KeyType, typename _ResType, typename ... _ArgTypes>
  using DefaultCommands = Commands<std::function, std::map, _KeyType, _ResType (_ArgTypes ...)>;
#endif // COMMANDS_INCLUDE_DEFAULT


  /*template<typename _KeyType, typename _ValueType>
  class IDictionary {
  public:
    virtual ~IDictionary() {}
    //...
  };//*/

}


#endif // VS_COMMANDS_HPP_INCLUDED
