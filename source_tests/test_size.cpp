#include <iostream>

#include "..\array2d.hpp"
#include "..\size.hpp"


using namespace std;
using namespace vslib;



int main()
{
  typedef Size<int> Size;
  Size s1 {18, 4};
  s1 = {5, 7};
  cout << s1.width << ", " << s1.height << endl;
  cout << s1.Area() << endl;

  cout << "\nDone." << endl;
  system("pause");
  return 0;
}
