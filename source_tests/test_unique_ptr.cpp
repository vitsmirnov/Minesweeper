#include <memory>
#include <iostream>


using namespace std;


class C1 {
public:
  C1(): _id(_id_counter++) {printf("C1 create [%i]\n", _id);}
  ~C1() {printf("C1 destroy [%i]\n", _id);}

  void print() const {printf("C1.print [%i]\n", _id);}
private:
  int _id{-1};

  static int _id_counter;// = 0;
};

int C1::_id_counter = 0;

int main()
{
  {
  unique_ptr<C1> cptr1{new C1};//= new C1;
  //cptr1 = new C1;
  cptr1->print();
  cptr1 = nullptr;
  printf("---\n");
  //cptr1->print();
  //cptr1 = new C1();
  cptr1.reset(new C1);
  cptr1->print();
  printf("%s\n", cptr1 == nullptr ? "null" : "not null");
  }

  printf("\nDone.\n");
  system("pause");
  return 0;
}
