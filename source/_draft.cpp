/// temp ///


/*LRESULT ???::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  printf("MainWindow::HandleMessage: msg = %i\n", uMsg);

  switch (uMsg) {
    case WM_DESTROY:
      PostQuitMessage(0);
      return 0;

    case WM_PAINT: {
      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(Window(), &ps);//_hwnd

      //FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));

      EndPaint(Window(), &ps);//_hwnd
    } return 0;

    default:
      return DefWindowProc(Window(), uMsg, wParam, lParam);//m_hwnd
  }
  return true;//??
}//*/


//DWORD wnd_style = WS_VISIBLE | WS_SYSMENU | WS_MINIMIZEBOX
//DWORD wnd_style = WS_VISIBLE | WS_OVERLAPPED | WS_BORDER | WS_SYSMENU
//DWORD wnd_style = WS_VISIBLE | WS_OVERLAPPEDWINDOW | !WS_THICKFRAME //| WS_OVERLAPPED //| WS_BORDER// | WS_SYSMENU

