
//#include <cstdlib>
//#include <cstdio>

#include <memory>
#include <functional>

#include "..\vs_command.hpp"


using namespace vslib;
using namespace std;


class C1 {
public:
  void Foo(void*) {
    printf("C1.Foo() is running, _field1 = %i\n", _field1);
  }
private:
  int _field1{79};
};

int main()
{
  function<void (C1*, void*)> f1 = C1::Foo;
  C1 c1;
  ICommand *command1;
  Command<C1> command2;
  command2.Setup(&c1, &C1::Foo);
  //command2.Setup(nullptr, nullptr);
  unique_ptr<ICommand> uptr1;

  command1 = &command2;
  (*command1)();

  printf("\nDone.\n");
  system("pause");
  return 0;
}
