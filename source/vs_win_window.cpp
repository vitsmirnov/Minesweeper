/*
  (c) Vitaly Smirnov [VSdev]
  mrmaybelately@gmail.com
  2024
*/


#include "vs_win_window.hpp"

#include <windows.h>
//#include <windowsx.h>

#include <string>
//#include <functional>
//#include <typeinfo>


namespace vslib //vswin
{

  /// class Window ///

  unsigned int Window::counter{0};


  Window::Window(const WindowData& data)
  {
    std::string class_name{ClassName() + std::to_string(counter)};

    WNDCLASSEX wincl{data.wnd_classex}; // Data structure for the window class
    wincl.hInstance = GetModuleHandle(nullptr);
    wincl.lpszClassName = class_name.data();
    wincl.lpfnWndProc = WindowProc;

    if (RegisterClassEx(&wincl) == 0) //ATOM
      throw "class Window construction: RegisterClassEx() failed";//?!

    _hwnd = CreateWindowEx(data.wnd_exstyle, class_name.data(), data.caption,
      data.wnd_style, data.x, data.y, data.width, data.height, data.wnd_parent,
      data.menu, wincl.hInstance, this);

    if (_hwnd == nullptr)
      throw "class Window construction: CreateWindowEx() failed";

    ++counter;
  }

  LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
  {
    Window *pThis = nullptr;

    if (uMsg == WM_NCCREATE) {//WM_CREATE ?
      CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
      pThis = (Window*)pCreate->lpCreateParams;
      SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

      pThis->_hwnd = hwnd;
    } else {
      pThis = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    }

    if (pThis)
      return pThis->HandleMessage(uMsg, wParam, lParam);
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
  }

  void Window::SetPositionToCenter()
  {
    RECT r, dtr;
    GetWindowRect(_hwnd, &r);
    GetWindowRect(GetDesktopWindow(), &dtr);
    SetPosition(
      ((dtr.right - dtr.left) - (r.right - r.left)) / 2,
      ((dtr.bottom - dtr.top) - (r.bottom - r.top)) / 2);
  }


  SIZE GetWindowFrameSize(HWND hwnd)
  {
    SIZE result;
    RECT r;
    GetWindowRect(hwnd, &r);
    //printf("GetWindowRect = (%d, %d, %d, %d)\n", r.left, r.top, r.right, r.bottom);
    result.cx = r.right - r.left;
    result.cy = r.bottom - r.top;
    GetClientRect(hwnd, &r);
    //printf("GetClientRect = (%d, %d, %d, %d)\n", r.left, r.top, r.right, r.bottom);
    result.cx -= r.right - r.left;
    result.cy -= r.bottom - r.top;

    return result;
  }


} // namespace vslib
