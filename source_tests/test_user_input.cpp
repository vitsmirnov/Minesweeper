
#include <cstdio>

#include "..\vs_user_input.hpp"


using namespace std;
using namespace vslib;


int main()
{
  UserInputEvent event = uiKeyUp;
  KeyCode key = VK_ESCAPE;
  ShiftState shift = MK_SHIFT;//0b10;
  InputEventCode code = EventToCode(event, key, shift);
  printf("event = %i, key = %i, shift state = %lli\nEvent code = %lli\n",
    event, key, shift, code);

  system("pause");
  return 0;
}
