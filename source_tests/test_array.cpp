#include <iostream>

#include "..\vs_array2d.hpp"
#include "..\vs_size.hpp"


using namespace std;
using namespace vslib;


template <typename T>
void PrintArr(Array2d<T> &arr, const char * name = "arr")
{
  char str1[255];
  sprintf(str1, "\n%s (%i, %i):\n", name, arr.GetWidth(), arr.GetHeight());
  arr.Print(" ", str1);
}

template <typename T>
void ArrTest1(const Array2d<T> &arr)
{
  //arr[0] = 5;
  cout << arr(0, 0) << endl;
  cout << arr[0] << endl;//"\nok..\n" << endl;
  cout << arr.At(0, 0) << endl;
}

int main()
{
  typedef Array2d<int> Int2dArr;
  //Array2d<int>
  //Int2dArr arr1(Int2dArr::ArraySize{10, 4}, 14);
  Int2dArr arr1({4, 3}, -14);

  //Size<> s1;

  //sprintf(str1, "\narr1 (%i, %i):\n", arr1.GetWidth(), arr1.GetHeight());
  //arr1.Print(" ", str1);
  arr1[3] = 1;
  PrintArr(arr1);
  arr1.Clear();
  PrintArr(arr1);//arr1.Print(" ", "\narr1:\n");
  arr1.SetSize({3, 5});
  PrintArr(arr1);//arr1.Print(" ", "\narr1:\n");
  arr1.Fill(8);
  PrintArr(arr1);
  arr1.Setup({2, 3}, 1);
  PrintArr(arr1);

  printf("\narr:\n");
  for (size_t i = 0; i < arr1.GetHeight(); ++i) {
    for (size_t j = 0; j < arr1.GetWidth(); ++j) {
      printf("[%i, %i, %i] ", j, i, arr1(j, i));
    }
    cout << endl;
  }

  const Int2dArr &arr5 = arr1;
  //Int2dArr &arr6 = (Int2dArr)arr5;
  //arr5[0] = 79;

  cout << "\n---------\n";
  arr1[0] = 77;
  ArrTest1(arr1);
  cout << "\n---------\n";

  cout << "\nDone." << endl;
  system("pause");
  return 0;
}
