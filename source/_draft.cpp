/// temp/draft ///


/// minesweeper_core.hpp
/*// The version with inheritance instead of composition
class GameField: public vslib::Array2d<FieldCell, Coord> {
public:
  using ParentClass = vslib::Array2d<FieldCell, Coord>;
public:
  static constexpr Size DefaultFieldSize{9, 9};//{10, 10};
  static constexpr Size MinFieldSize{4, 4};
  static constexpr Size MaxFieldSize{1000, 1000};
  static constexpr FieldCell DefaultCell{false, Cell::Status::Closed};
public:
  explicit GameField(const Size& size = DefaultFieldSize, FieldCell cell = DefaultCell):
    ParentClass::Array2d{ValidatedSize(size), cell} {}

  bool Setup(const Size& size, FieldCell cell = DefaultCell)
    {return ParentClass::Setup(ValidatedSize(size), cell);}

  bool SetSize(const Size& new_size)
    {return ParentClass::SetSize(ValidatedSize(new_size));}
  bool SetSizeIfPossible(const Size& new_size)
    {return IsSizeValid(new_size) && ParentClass::SetSize(new_size);}

  static Size ValidatedSize(Size size)
    {return size.FitInto(MinFieldSize, MaxFieldSize);}
  static bool IsSizeValid(const Size& value) {
    return
      value.width >= MinFieldSize.width && value.height >= MinFieldSize.height &&
      value.width <= MaxFieldSize.width && value.height <= MaxFieldSize.height;
  }

  using ParentClass::operator[];
  FieldCell& operator[](const Point& pos) //operator()
    {return ParentClass::operator()(pos.x, pos.y);}
  const FieldCell& operator[](const Point& pos) const //operator()
    {return ParentClass::operator()(pos.x, pos.y);}

  template <typename _TypeName>
  int CountAround(Coord x, Coord y,
    _TypeName FieldCell::*field, _TypeName value) const; // inline?
  int MinesAround(Coord x, Coord y) const
    {return CountAround(x, y, &FieldCell::has_mine, true);}//<bool>
  int MineMarkedAround(Coord x, Coord y) const
    {return CountAround(x, y, &FieldCell::status, Cell::Status::MineMarked);}//<CellStatus>

  inline Rect GetRoundRect(Coord x, Coord y) const;
  Rect GetRoundRect(const Point& p) const
    {return GetRoundRect(p.x, p.y);}
};//*/


/// vs_user_input.hpp
/*
//The first version
// Info:
//InputEventCode: XX (-type) XXXX (-key code) XXXX (-shift state)
// XX_XXXX_XXXX
//
// Main info:
//  event
//  key code
//  shift state
// Extra info:
//  mouse pos
//  mouse wheel shift
//
//uns int (4) 4_294_967_296
//uns long long (8) 18_446_744_073_709_552_000 = 1.8446744e+19
//


//using InputEventCode = unsigned long long;

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
}


enum UserInputType: char {uiKeyDown, uiKeyUp, uiMouseDown, uiMouseUp, uiMouseMove, uiMouseDblClick};//uiMouseWheel(Up/Down), uiKeyPress

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

