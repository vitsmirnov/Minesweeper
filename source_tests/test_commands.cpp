#include "..\vs_commands.hpp"


using namespace vslib;


class Cl1 {
public:
  void f1(void* data) {printf("Cl1.f1()\n");}
  void f2(void*) {printf("Cl1.f2()\n");}
  void f3(int n) {printf("Cl1.f3(%i)\n", n);}
  void f4(int n) {printf("Cl1.f4(%i)\n", n);}
};


int main()
{
  Cl1 c1;
  Commands<Cl1, unsigned char, void, void> commands1(&c1);
  //commands1.SetObject(nullptr);
  commands1.SetCommand(0, &c1.f1);
  commands1.SetCommand(1, &c1.f2);
  commands1.DoCommand(0);
  commands1.DoCommand(1);
  commands1.DoCommand(10);
  //commands1.SetCommand(1, nullptr);
  commands1.DeleteCommand(10);

  printf("\n");
  commands1.SetCommand(2, &c1.f2);
  commands1.DeleteCommand(c1.f2);
  commands1.DoCommand(1);
  commands1.DoCommand(2);
  commands1[1];

  printf("\nDone.\n");
  system("pause");
  return 0;
}
