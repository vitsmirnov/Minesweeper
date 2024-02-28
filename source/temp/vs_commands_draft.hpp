#ifndef VS_COMMANDS_HPP_INCLUDED
#define VS_COMMANDS_HPP_INCLUDED


#include <map>


//using namespace std;


namespace vslib
{

  template <typename _ObjectType, typename _KeyType = unsigned int,
    typename _ArgType = void, typename _ResType = void>//, _ArgType _DefaultArg = void>
  class Commands {
  public:
    typedef _ResType (_ObjectType::*Method)(_ArgType*);// = _DefaultArg);
  public:
    explicit Commands(_ObjectType* object = nullptr): _object(object) {}
    //Commands(const Commands& commands) = delete;
    //void operator=(const Commands& commands) = delete;
    //virtual ~Commands() {}

    void SetObject(_ObjectType* object) {_object = object;}
    void SetCommand(const _KeyType& key, Method method) {_methods[key] = method;}
    void DeleteCommand(const _KeyType& key) {_methods.erase(key);}
    void DeleteCommand(Method method) { // This should be checked!
      for (auto &it: _methods)
        if (it.second == method)
          _methods.erase(it.first);
    }

    void DoCommand(const _KeyType& key, _ArgType* arg = nullptr) { //rename to Execute?
      if (!_object)
        return;
      auto method_it = _methods.find(key);
      if (method_it != _methods.end() && method_it->second)
        (_object->*(method_it->second))(arg);
      //(_object->*_methods[key])(arg);
    }

    void operator[](_KeyType key) {DoCommand(key);} //?
  private:
    _ObjectType* _object{nullptr};
    std::map<_KeyType, Method> _methods;// It should be an interface!
  };

}


#endif // VS_COMMANDS_HPP_INCLUDED
