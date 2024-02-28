/*
  (c) Vitaly Smirnov [VSdev]
  mrmaybelately@gmail.com
  2023
*/

#ifndef VS_COMMAND_HPP_INCLUDED
#define VS_COMMAND_HPP_INCLUDED


//#include <functional>


namespace vslib
{

  class ICommand {
  public:
    virtual ~ICommand() {};//= 0;//{}?
    virtual void operator()(void* data = nullptr) = 0;
  };


  template <typename _Type>
  class Command: public ICommand {
  public:
    typedef _Type ObjectType;
    typedef void (_Type::*PFunction)(void* data);//(P)Method?
    using Method = void (_Type::*)(void*);
  public:
    Command() = default;
    Command(_Type* object, PFunction func): _object(object), _function(func) {}
    //virtual ~ICommand() = 0;

    void Setup(_Type* object, PFunction func) {_object = object; _function = func;}

    virtual void operator()(void* data = nullptr) override {
      if (_object && _function)
        try {// Do we need this?
          (_object->*_function)(data);
        } catch (...) {}
    }
  private:
    _Type* _object{nullptr};
    PFunction _function{nullptr};
    //std::function<> ??
  };

}


#endif // VS_COMMAND_HPP_INCLUDED
