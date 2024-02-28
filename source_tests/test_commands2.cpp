
#include <map>
#include <stack>

#include "..\vs_commands.hpp"
#include "..\vs_method_ptr.hpp"
//#include "..\vs_any_function.hpp"


using namespace std;
using namespace vslib;
//using namespace vslib_x;


class Cl1 {
public:
  int f1(void* data) {
    printf("Cl1.f1()\n");
    return -100;
  }
  int f2(void*) {
    printf("Cl1.f2()\n");
    return -200;
  }
  int f3(int n) {
    printf("Cl1.f3(%i)\n", n);
    return -300;
  }
  int f4(int n) {
    printf("Cl1.f4(%i)\n", n);
    return -400;
  }
};

int Func1(int n)
{
  printf("Func1(%i)\n", n);
  return n + 1000;
}


int main()
{
  Cl1 c1;
  //using Commands1 = Commands<std::map, unsigned int, int (int)>;
  //using Commands1 = Commands<std::map<unsigned int, std::function<int (int)>>, unsigned int, int (int)>;
  //using Commands1 = Commands<map, unsigned int, int (int)>;
  using Commands1 = Commands<function, map, unsigned int, int (int)>;
  //using Commands1 = DefaultCommands<unsigned int, int, int>;
  //using Commands1 = Commands<AnyFunctionPtr, map, unsigned int, int (int)>;
  Commands1 commands1;
  commands1.SetCommand(0, MakeMethod(&c1, c1.f4));
  commands1.SetCommand(1, Func1);
  auto l1 = [](int n){printf("Lambda(%i)\n", n); return n - 1000;};
  commands1.SetCommand(11, l1);
  //commands1.Clear();

  commands1.DoCommand(0, 10);
  commands1.DoCommand(1, 20);
  commands1.DoCommand(10, 30);
  commands1.DoCommand(11, 40);

  //commands1.SetCommand(1, nullptr);
  commands1.DeleteCommand(10);


  system("pause");
  return 0;
}
