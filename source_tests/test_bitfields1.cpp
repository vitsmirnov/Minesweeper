//#include <conio.h>
#include <cstdio>
#include <cstdlib>

#include <windows.h>
#include <windowsx.h>

#include <bitset>
#include <iostream>


using namespace std;


struct byte2 {
  unsigned a0: 1;
  unsigned a1: 1;
  unsigned a2: 1;
  unsigned a3: 1;
  unsigned a4: 1;
  unsigned a5: 1;
  unsigned a6: 1;
  unsigned a7: 1;
};

union ShiftState {
  unsigned main;
  struct {
    unsigned a0: 1;
    unsigned a1: 1;
    unsigned a2: 1;
    unsigned a3: 1;
    unsigned a4: 1;
    unsigned a5: 1;
    unsigned a6: 1;
    unsigned a7: 1;
  };
  //byte2 bits;
};

int main()
{
  byte2 x = {0, 0, 0, 1, 0, 0, 0, 0};
  x.a7 = 1;

  printf("WPARAM = %lli, LPARAM = %lli\n", sizeof(WPARAM), sizeof(LPARAM));
  printf("MK_SHIFT = %lli\n\n", sizeof((short)MK_SHIFT));

  /*printf("sizeof byte = %lli\n", sizeof(byte));
  printf("x.a0 = %d\n", x.a0);
  printf("x.a1 = %d\n", x.a1);
  printf("x.a2 = %d\n", x.a2);
  printf("x.a3 = %d\n", x.a3);
  printf("x.a4 = %d\n", x.a4);
  printf("x.a5 = %d\n", x.a5);
  printf("x.a6 = %d\n", x.a6);
  printf("x.a7 = %d\n", x.a7);//*/

  ShiftState s1 = {0};
  //s1.main = (unsigned)MK_LBUTTON;
  //s1.main = (unsigned)MK_RBUTTON;
  s1.main = (unsigned)MK_SHIFT | (unsigned)MK_LBUTTON;
  //s1.main = (unsigned)MK_CONTROL;
  //s1.main = (unsigned)MK_MBUTTON;
  //s1.main = (unsigned)MK_XBUTTON1;
  //s1.main = (unsigned)MK_XBUTTON2;
  printf("sizeof ShiftState = %lli\n", sizeof(ShiftState));
  printf("a0 = %d\n", s1.a0);
  printf("a1 = %d\n", s1.a1);
  printf("a2 = %d\n", s1.a2);
  printf("a3 = %d\n", s1.a3);
  printf("a4 = %d\n", s1.a4);
  printf("a5 = %d\n", s1.a5);
  printf("a6 = %d\n", s1.a6);
  printf("a7 = %d\n", s1.a7);

  signed char c1 = -127;
  for (int i = 0; i < 8; ++i) {
    c1 = c1 >> 1;
    cout << bitset<8>(c1) << " = " << (int)c1 << "\n";
  }

  system("pause");
  return 0;
}
