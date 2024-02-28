/*
  (c) Vitaly Smirnov [VSdev]
  mrmaybelately@gmail.com
  2023-2024
*/

#ifndef VS_USER_INPUT_H_INCLUDED
#define VS_USER_INPUT_H_INCLUDED


#include <winuser.h>
//#include <windows.h>
//#include <windowsx.h>


namespace vslib
{

  enum class UserInputEvent: unsigned short {//char
    //Undefined = 0 ?? //Unknown? // Invalid?
    KeyDown = 1,
    KeyUp,
    KeyPress, // for WM_CHAR message
    MouseDown,
    MouseUp,
    MouseDblClick,
    MouseMove,
    MouseWheel
  };

  using KeyCode = unsigned short;

  using KeysState = unsigned short;//WPARAM;
  //enum class KeyState {sShift, sCtrl, sAlt, ..};

  struct UserInput {
  public:
    UserInputEvent event{(UserInputEvent)0};// top
    KeyCode key{0}; // middle
    KeysState keys_state{0}; // bottom

    bool operator==(const UserInput& other) const
      {return event == other.event && key == other.key && keys_state == other.keys_state;}
    bool operator!=(const UserInput& other) const
      {return !operator==(other);}
    bool operator>(const UserInput& other) const {
      /*return
        (event > other.event) ||
        (event == other.event && key > other.key) ||
        (event == other.event && key == other.key && keys_state > other.keys_state);//*/
      // It should be faster
      if (event != other.event)
        return event > other.event;
      if (key != other.key)
        return key > other.key;
      if (keys_state != other.keys_state)
        return keys_state > other.keys_state;
      return false;
    }
    bool operator<(const UserInput& other) const {
      if (event != other.event)
        return event < other.event;
      if (key != other.key)
        return key < other.key;
      if (keys_state != other.keys_state)
        return keys_state < other.keys_state;
      return false;
    }
    bool operator>=(const UserInput& other) const
      {return !operator<(other);}
    bool operator<=(const UserInput& other) const
      {return !operator>(other);}
  };


  /// functions ///

  inline UserInputEvent WinMsgToEvent(UINT msg)
  {
    switch (msg) {
      case WM_KEYDOWN:
        return UserInputEvent::KeyDown;
      case WM_KEYUP:
        return UserInputEvent::KeyUp;
      case WM_CHAR:
        return UserInputEvent::KeyPress;
      case WM_LBUTTONDOWN:
      case WM_RBUTTONDOWN:
      case WM_MBUTTONDOWN:
        return UserInputEvent::MouseDown;
      case WM_LBUTTONUP:
      case WM_RBUTTONUP:
      case WM_MBUTTONUP:
        return UserInputEvent::MouseUp;
      case WM_LBUTTONDBLCLK:
      case WM_RBUTTONDBLCLK:
      case WM_MBUTTONDBLCLK:
        return UserInputEvent::MouseDblClick;
      case WM_MOUSEMOVE:
        return UserInputEvent::MouseMove;
      case WM_MOUSEWHEEL:
        return UserInputEvent::MouseWheel;
      default:
        return (UserInputEvent)0;
    }
  }

  inline KeyCode WinMsgToMouseButton(UINT msg)
  {
    switch (msg) {
      case WM_LBUTTONDOWN:
      case WM_LBUTTONUP:
      case WM_LBUTTONDBLCLK:
        return VK_LBUTTON;
      case WM_RBUTTONDOWN:
      case WM_RBUTTONUP:
      case WM_RBUTTONDBLCLK:
        return VK_RBUTTON;
      case WM_MBUTTONDOWN:
      case WM_MBUTTONUP:
      case WM_MBUTTONDBLCLK:
        return VK_MBUTTON;
      default:
        return 0;
    }
  }



  /* // The first version
  // Info:
  //InputEventCode: XX (-type) XXXX (-key code) XXXX (-shift state)
  // XX_XXXX_XXXX

  // Main info:
  //  event
  //  key code
  //  shift state
  // Extra info:
  //  mouse pos
  //  mouse wheel shift

  //uns int (4) 4_294_967_296
  //uns long long (8) 18_446_744_073_709_552_000 = 1.8446744e+19
  //*/

  /*//using InputEventCode = unsigned long long;

  //using KeysState = WPARAM;

  inline InputEventCode EventToCode(UserInputEvent event, KeyCode key, ShiftState shift = 0)
  {
    // InputEventCode: XX (-type) XXXX (-key code) XXXX (-shift state)
    // XX_XXXX_XXXX

    //InputEventCode result = event * 100000000;
    //result += key * 10000;
    //result = result | shift;
    //return result;

    return ((event * 100000000) + (key * 10000)) | shift;
  }//*/


}


/*enum UserInputType: char {uiKeyDown, uiKeyUp, uiMouseDown, uiMouseUp, uiMouseMove, uiMouseDblClick};//uiMouseWheel(Up/Down), uiKeyPress

struct UserInput {
public:
  UserInputType Type;
  union {
    short KeyCode; //unsigned char
    POINTS MousePos;
    //ShiftState??
  }
};

key code
shift state
mouse pos
mouse wheel shift

InputEventCode: XX (-type) XXXX (-key code)

int (4) 4_294_967_296
long (8) 18_446_744_073_709_552_000
1.8446744e+19

//*/


#endif // VS_USER_INPUT_H_INCLUDED
