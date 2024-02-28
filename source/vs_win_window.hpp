/*
  (c) Vitaly Smirnov [VSdev]
  mrmaybelately@gmail.com
  2024
*/

#ifndef VS_WIN_WINDOW_HPP_INCLUDED
#define VS_WIN_WINDOW_HPP_INCLUDED


#include <windows.h>
//#include <windowsx.h>

#include <functional>
#include <typeinfo>


namespace vslib //vswin
{

  /// classes list ///
  struct WindowData;

  class Window;
  class ControlledWindow;
  class WinController;


  /// struct WindowData ///

  // struct for default initialization for Window class
  struct WindowData {//WindowInitData
  public:
    WNDCLASSEX wnd_classex{
      sizeof(wnd_classex),
      CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW,
      nullptr,  // It is ignored in Window class
      0,
      0,
      nullptr,  // It is ignored in Window class
      LoadIcon(nullptr, IDI_APPLICATION),
      LoadCursor(nullptr, IDC_ARROW),
      (HBRUSH)COLOR_WINDOW,
      nullptr,
      nullptr,  // It is ignored in Window class
      LoadIcon(nullptr, IDI_APPLICATION)
    };

    DWORD wnd_exstyle{0};
    //LPCSTR class_name{nullptr};  // It is ignored in Window class
    LPCSTR caption{nullptr};
    DWORD wnd_style{WS_OVERLAPPEDWINDOW | WS_VISIBLE};
    int x{CW_USEDEFAULT};
    int y{CW_USEDEFAULT};
    int width{CW_USEDEFAULT};
    int height{CW_USEDEFAULT};
    HWND wnd_parent{nullptr};
    HMENU menu{nullptr};
    //HINSTANCE instance{nullptr};  // It is ignored in Window class
    //LPVOID lp_param{nullptr};  // It is ignored in Window class
  public:
    WindowData() = default;
    // constructor for the values which need to be initialized more often
    explicit WindowData(LPCSTR _caption, DWORD _wnd_style = WS_OVERLAPPEDWINDOW | WS_VISIBLE,
      int _x = CW_USEDEFAULT, int _y = CW_USEDEFAULT,
      int _width = CW_USEDEFAULT, int _height = CW_USEDEFAULT,
      HBRUSH _bg_color = (HBRUSH)COLOR_WINDOW):
        caption{_caption}, wnd_style{_wnd_style}, x{_x}, y{_y},
        width{_width}, height{_height}
    {
      wnd_classex.hbrBackground = _bg_color;
    }
  };


  SIZE GetWindowFrameSize(HWND hwnd);


  // There should be a SuperClass (root) for Window (like BasicWindow). Or not?


  /// class Window ///

  class Window {//BasicWindow
  public:
    Window(const WindowData& data = WindowData{});//explicit?
    virtual ~Window() {DestroyWindow(_hwnd);}//??

    const HWND GetHandle() const {return _hwnd;}//const? //GetData() ? Data() ?

    void ShowAs(int cmd_show) {ShowWindow(_hwnd, cmd_show);}
    void ShowDefault() {ShowWindow(_hwnd, SW_SHOWDEFAULT);}
    void ShowNormal() {ShowWindow(_hwnd, SW_SHOWNORMAL);}
    void Show() {ShowWindow(_hwnd, SW_SHOW);}//SW_SHOWNORMAL
    void Hide() {ShowWindow(_hwnd, SW_HIDE);}
    void SetCaption(LPCSTR caption) {SetWindowText(_hwnd, caption);}
    //LPCSTR GetCaption() const {return GetWindowText(_hwnd, );}//std::string
    void SetPosition(const POINTS& pos) {SetPosition(pos.x, pos.y);}
    void SetPosition(short x, short y) { // It won't work correctly for x/left!
      SetWindowPos(_hwnd, HWND_TOP, x, y, 0, 0,
        SWP_NOSIZE | SWP_NOZORDER);// | SWP_FRAMECHANGED
      //MoveWindow(_hwnd, x, y, 0, 0, true);
    }
    void SetPositionToCenter();
    void SetSize(const SIZE& new_size) {SetSize(new_size.cx, new_size.cy);}
    void SetSize(LONG width, LONG height) {
      SetWindowPos(_hwnd, HWND_TOP, 0, 0, width, height,
        SWP_NOMOVE | SWP_NOZORDER);// | SWP_FRAMECHANGED
    }
    void SetClientSize(const SIZE& new_size) {SetClientSize(new_size.cx, new_size.cy);}
    void SetClientSize(LONG width, LONG height) {
      SIZE s{GetWindowFrameSize(_hwnd)};
      SetWindowPos(_hwnd, HWND_TOP, 0, 0, width + s.cx, height + s.cy,
        SWP_NOMOVE | SWP_NOZORDER);// | SWP_FRAMECHANGED
    }
    //void SetLeft()
    //void SetTop()
    //void SetWidth()
    //void SetHeight()
    //void SetClientWidth()
    //void SetClientHeight()

    // Do we need this?
    virtual const char* ClassName() const //= 0;//LPCSTR/PCWSTR // Can we do it as static??
      {return typeid(*this).name();}
  protected:
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
      {return DefWindowProc(_hwnd, uMsg, wParam, lParam);}
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
  protected: // Deprecated
    /*bool Create(
      LPCSTR caption = "",//ClassName(),
      int x = CW_USEDEFAULT,
      int y = CW_USEDEFAULT,
      int width = CW_USEDEFAULT,
      int height = CW_USEDEFAULT,
      DWORD style = WS_OVERLAPPEDWINDOW,// | WS_VISIBLE,
      DWORD ex_style = 0,
      HWND wnd_parent = nullptr,//0,
      HMENU menu = nullptr//0
    )
    {
      if (!RegisterWindowClass())
        return false;//?!

      _hwnd = CreateWindowEx(ex_style, ClassName(), caption, style, x, y,
        width, height, wnd_parent, menu, GetModuleHandle(nullptr), this);

      return _hwnd != nullptr;
    }//*/
    /*bool RegisterWindowClass(//HINSTANCE hThisInstance
      HBRUSH bg_color = (HBRUSH)COLOR_WINDOW, // Default color ?
      UINT style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW, // Catch double-clicks
      HCURSOR cursor = LoadCursor(nullptr, IDC_ARROW), // Default cursor
      HICON icon = LoadIcon(nullptr, IDI_APPLICATION), // Default icon
      HICON small_icon = LoadIcon(nullptr, IDI_APPLICATION), // Default small icon
      LPCSTR menu_name = nullptr, // No menu
      int cls_extra = 0, // No extra bytes after the window class
      int wnd_extra = 0) // structure or the window instance
    {
      WNDCLASSEX wincl{0}; // Data structure for the window class
      wincl.cbSize = sizeof(wincl);
      wincl.hInstance = GetModuleHandle(nullptr);//hThisInstance;
      wincl.lpszClassName = ClassName();
      wincl.lpfnWndProc = _DerivedType::WindowProc;// This function is called by windows

      wincl.hbrBackground = bg_color;//(HBRUSH)COLOR_WINDOW;//BACKGROUND; // Use Windows's default colour as the background of the window
      wincl.style = style;// | CS_OWNDC;
      wincl.hCursor = cursor;
      wincl.hIcon = icon;
      wincl.hIconSm = small_icon;
      wincl.lpszMenuName = menu_name;
      wincl.cbClsExtra = cls_extra;
      wincl.cbWndExtra = wnd_extra;

      // Register the window class, and if it fails quit the program
      return RegisterClassEx(&wincl) != 0;
    }//*/
    /*bool Create2(
      // For RegisterClassEx
      HBRUSH bg_color = (HBRUSH)COLOR_WINDOW, // Default color ?
      UINT class_style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW, // Catch double-clicks
      HCURSOR cursor = LoadCursor(nullptr, IDC_ARROW), // Default cursor
      HICON icon = LoadIcon(nullptr, IDI_APPLICATION), // Default icon
      HICON small_icon = LoadIcon(nullptr, IDI_APPLICATION), // Default small icon
      LPCSTR menu_name = nullptr, // No menu
      int cls_extra = 0, // No extra bytes after the window class
      int wnd_extra = 0, // structure or the window instance
      // For CreateWindowEx
      LPCSTR caption = "",//ClassName(),
      int x = CW_USEDEFAULT,
      int y = CW_USEDEFAULT,
      int width = CW_USEDEFAULT,
      int height = CW_USEDEFAULT,
      DWORD wnd_style = WS_OVERLAPPEDWINDOW | WS_VISIBLE,
      DWORD wnd_exstyle = 0,
      HWND wnd_parent = nullptr,//0,
      HMENU menu = nullptr)//0
    {
      std::string class_name{ClassName() + std::to_string(counter)};
      WNDCLASSEX wincl{0}; // Data structure for the window class

      wincl.cbSize = sizeof(wincl);
      wincl.hInstance = GetModuleHandle(nullptr);//hThisInstance;
      wincl.lpszClassName = class_name.data();
      wincl.lpfnWndProc = WindowProc; // This function is called by windows

      wincl.hbrBackground = bg_color;//(HBRUSH)COLOR_WINDOW;//BACKGROUND; // Use Windows's default colour as the background of the window
      wincl.style = class_style;// | CS_OWNDC;
      wincl.hCursor = cursor;
      wincl.hIcon = icon;
      wincl.hIconSm = small_icon;
      wincl.lpszMenuName = menu_name;
      wincl.cbClsExtra = cls_extra;
      wincl.cbWndExtra = wnd_extra;

      if (RegisterClassEx(&wincl) == 0) //ATOM
        return false;//?!

      _hwnd = CreateWindowEx(wnd_exstyle, class_name.data(), caption, wnd_style,
        x, y, width, height, wnd_parent, menu, wincl.hInstance, this);

      return _hwnd != nullptr;
    }//*/
  private:
    HWND _hwnd{nullptr};
  private:
    static unsigned int counter; // for identification
  };


  /// class ControlledWindow ///

  using WindowProcFunc = std::function<LRESULT (HWND, UINT, WPARAM, LPARAM)>;

  class ControlledWindow: public Window {//Bad name!
  public:
    using Window::Window;
    explicit ControlledWindow(const WindowProcFunc& handler,
      const WindowData& data = WindowData{}):
        Window::Window{data}, _messages_handler(handler) {}

    const WindowProcFunc& GetMessagesHandler() const
      {return _messages_handler;}
    void SetMessagesHandler(const WindowProcFunc& handler)//nullptr_t?
      {_messages_handler = handler;}
  protected:
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override {
      if (_messages_handler != nullptr)
        return _messages_handler(GetHandle(), uMsg, wParam, lParam);
      return Window::HandleMessage(uMsg, wParam, lParam);//DefWindowProc(GetHandle(), uMsg, wParam, lParam);
    }
  private:
    WindowProcFunc _messages_handler{nullptr};
  };


  /// class WinController

  class WinController {//WinController? It could be used only for Windows
  public:
    virtual ~WinController() {}
    virtual WindowProcFunc GetWindowProc() = 0;//const?
    //virtual vslib::Method<Controller, LRESULT (HWND, UINT, WPARAM, LPARAM)> GetWindowProc() = 0;
    //virtual LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param)
      //{return DefWindowProc(hwnd, msg, w_param, l_param);}
  };//*/



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

} // namespace vslib


#endif // VS_WIN_WINDOW_HPP_INCLUDED
