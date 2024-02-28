/*
  (c) Vitaly Smirnov [VSdev]
  mrmaybelately@gmail.com
  2023
*/

#ifndef MINESWEEPERENGINE_H_INCLUDED
#define MINESWEEPERENGINE_H_INCLUDED


#include <windows.h>
#include <windowsx.h>
//#include <gdiplus.h>

#include <stdio.h>

//#include "vs_points.h"
//#include "vs_list.h"
#include "vs_math.h"

//using namespace vslib;

//extern RECT ClientRect;

constexpr short OneByte = 256;
constexpr short TwoBytes = 512;

// ???
extern const COLORREF ColorWhite;// = RGB(255, 255, 255);
extern const COLORREF ColorBlack;// = RGB(0, 0, 0);


struct Size;
struct Cell;
class Field;
class SapperGame;
class SapperGameExecutor;


typedef POINTS ShortPoint;


inline bool operator==(ShortPoint aPos1, ShortPoint aPos2);
inline bool operator!=(ShortPoint aPos1, ShortPoint aPos2);
inline bool IsPointInRect(ShortPoint aPoint, const RECT &aRect);

inline bool DrawRect(HDC aHdc, const RECT &aRect);
inline bool DrawEllipse(HDC aHdc, const RECT &aRect);
void DrawFlag(HDC aHdc, const RECT &aRect);
inline void SetDCBrushAndPenColors(HDC aHdc, COLORREF aBrushColor, COLORREF aPenColor);
inline bool DrawRect(HDC aHdc, const RECT &aRect, COLORREF aBrushColor, COLORREF aPenColor);
inline bool DrawEllipse(HDC aHdc, const RECT &aRect, COLORREF aBrushColor, COLORREF aPenColor);
inline void DrawFlag(HDC aHdc, const RECT &aRect, COLORREF aBrushColor, COLORREF aPenColor);

const Size& GetWindowSizeCoeffs(HWND aHwnd, bool UpdateCoeffs = false);
inline void SetWindowClientSize(HWND aHwnd, int aWidth, int aHeight, bool aUpdateCoeffs = false);



struct Size {
public:
  int Width;//int ?? size_t?
  int Height;
public:
  /*explicit*/ Size(int aWidth = 0, int aHeight = 0): Width(aWidth), Height(aHeight) {}

  void Setup(int aWidth, int aHeight) {Width = aWidth; Height =aHeight;}
  void operator()(int aWidth, int aHeight) {Setup(aWidth, aHeight);}

  bool operator==(const Size &aSize) const
    {return Width == aSize.Width && Height == aSize.Height;}
  bool operator!=(const Size &aSize) const
    {return !operator==(aSize);}

  int Area() const //GetArea
    {return Width*Height;}
  bool IsPointInRect(int aX, int aY) const
    {return aX >= 0 && aY >= 0 && aX < Width && aY < Height;}
  bool IsPointInRect(ShortPoint aPos) const
    {return IsPointInRect(aPos.x, aPos.y);}
  // ??
  bool IsValid() const
    {return Width >=0 && Height >= 0;}
  bool IsZero() const
    {return Width == 0 || Height == 0;}
};


enum CellStatus/*: char*/ {csClosed, csMineMarked, csOpened};//, csQuestionMarked};

struct Cell {
public://private:
  bool IsMine;// Rename to HasMine!
  CellStatus Status;
  //char Number;
public:
  Cell(bool aIsMine = false, CellStatus aStatus = csClosed):
    IsMine(aIsMine), Status(aStatus) {}

  void Setup(bool aIsMine, CellStatus aStatus)
    {IsMine = aIsMine; Status = aStatus;}
  void operator()(bool aIsMine, CellStatus aStatus)
    {Setup(aIsMine, aStatus);}

  bool Open();
  bool MineMarking();

  void Draw(HDC aHdc, const RECT &aRect, int *aMinesAround = 0);
};


class Field {
private:
  Cell **mCells{nullptr};
  Size mSize{0, 0};
public:
  explicit Field(const Size &aSize = {10, 10}, bool aIsMine = false,
    CellStatus aCellStatus = csClosed)//: mCells(0), mSize(0, 0)
  {
    Setup(aSize, aIsMine, aCellStatus);
  }
  ~Field()
    {Destroy();}

  void Destroy();
  void Reset(bool aIsMine = false, CellStatus aCellStatus = csClosed);//Fill?
  bool Setup(const Size &aSize,
    bool aIsMine = false, CellStatus aCellStatus = csClosed);

  Size GetSize() const//const Size&
    {return mSize;}

  inline Cell& operator[](int aIndex);
  inline Cell& operator()(int aX, int aY);

  inline RECT GetRoundRect(int aX, int aY);
  template <typename _TypeName>
  int CountAround(int aX, int aY, _TypeName Cell::*PField, _TypeName aValue);
  int MinesAround(int aX, int aY)
    {return CountAround<bool>(aX, aY, &Cell::IsMine, true);}
  int MineMarkedAround(int aX, int aY)
    {return CountAround<CellStatus>(aX, aY, &Cell::Status, csMineMarked);}

  void Draw(HDC aHdc, const Size &aCellSize);
};


typedef LRESULT CALLBACK (*PWindowProc)(HWND, UINT, WPARAM, LPARAM);

class SapperGame {
private://public:
  typedef void (SapperGame::*PPlayerCommand)();//(void *aData)
private:
  HWND mHwnd;
  int mCmdShow;

  PPlayerCommand mPlayerCommands[OneByte*2];

  Field mField;
  int mMinesCount;
  Size mCellSize;
  // Runtime data
  int mMineMarked;
  int mCellOpened;
  //int mGameTime;
  bool mIsGameRunning;

  /*int mCellWidth = 30;
  int mCellHeight = 30;
  int mFieldWidth = 10;
  int mFieldHeight = 15;
  int mScreenWidth = mFieldWidth * mCellWidth;
  int mScreenHeight = mFieldHeight * mCellHeight;//*/
protected:// Player commands
  void Quit()
    {PostQuitMessage(0);}
  void Test1() {
    SetWindowClientSize(mHwnd,
      mField.GetSize().Width * mCellSize.Width, mField.GetSize().Height * mCellSize.Height, true);
  }
protected:
  void RunGameLoop();

  bool IsKeyCodeCorrect(short aKeyCode)
    {return aKeyCode >= 0 && aKeyCode < (short)(sizeof(mPlayerCommands) / sizeof(PPlayerCommand));}
  void DoPlayerCommand(short aKeyCode) {
    if(IsKeyCodeCorrect(aKeyCode) && mPlayerCommands[aKeyCode])
      (this->*mPlayerCommands[aKeyCode])();
  }

  void GetAndDoPlayerCommand(bool &aBreakGameLoop)
    {ProcessMessages(aBreakGameLoop);}
  void UpdateGameLogic(int aDeltaTime);
  void UpdateScreen(HDC aDc);
  void Repaint(HWND aHwnd);

  void ProcessMessages(bool &aQuit);//bool
protected:
  static bool IsMineCountValid(int aMineCount, const Size &aFieldSize)
    {return aMineCount > 0 && aMineCount <= aFieldSize.Area() - 9;}
  ShortPoint PosToPoint(int aPos) const
    {return {(short)(aPos % mField.GetSize().Width), (short)(aPos / mField.GetSize().Width)};}
  inline bool IsPosCorrect(ShortPoint aPos, ShortPoint aStartPos);

  bool PutMines(ShortPoint aStartPos, int aMineCount); //Reset
  void OpenCell(int aX, int aY);
  void MineMarkedOrClose(int aX, int aY);
  void OpenAround(int aX, int aY);
  void StartGame(ShortPoint aPos);

  Size GetFieldScreenSize() const
    {return {mField.GetSize().Width * mCellSize.Width, mField.GetSize().Height * mCellSize.Height};}
public:
  SapperGame(HINSTANCE aInstance, int aCmdShow, PWindowProc aWindowProc,
    const Size &aFieldSize = {10, 10}, int aMinesCount = 10, const Size &aCellSize = {30, 30});
  ~SapperGame();

  void Setup(const Size &aFieldSize, int aMinesCount = 10) {
    mField.Setup(aFieldSize, false, csClosed);
    mMinesCount = aMinesCount;
    mMineMarked = 0;
    mCellOpened = 0;
  }
  void NewGame() {
    mField.Reset(false, csClosed);
    mMineMarked = 0;
    mCellOpened = 0;
  }
  void Run();

  //Size& CellSize() {return mCellSize;}
  const Size& GetCellSize() const
    {return mCellSize;}
  void SetCellSize(const Size &aValue)
    {mCellSize = aValue; UpdateScreen(GetDC(mHwnd));}

  LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};


class SapperGameExecutor { //Runner
private:
  static SapperGame *smGame;
protected:
  static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    //if(!smGame)
      //return DefWindowProc(hwnd, uMsg, wParam, lParam);
    return smGame->WindowProc(hwnd, uMsg, wParam, lParam);
  }
public:
  SapperGameExecutor(HINSTANCE hInstance, int nCmdShow) {
    if(smGame)//smDoesItExist)
      throw "DinoGameExecutor already exists and it can only exist in single copy.";
    smGame = new SapperGame(hInstance, nCmdShow, &WindowProc);
  }
  ~SapperGameExecutor() {
    delete smGame;
    smGame = 0;
  }

  void Run() {smGame->Run();}
};


/// class Field ///

inline Cell& Field::operator[](int aIndex)
{
  if(aIndex < 0 || aIndex >= mSize.Area())
    throw "Field.operator[int]: range check error!";
  return (*mCells)[aIndex];
}

inline Cell& Field::operator()(int aX, int aY)
{
  if(!mSize.IsPointInRect(aX, aY))
    throw "Field.operator(int, int): range check error!";
  return mCells[aX][aY];
}

inline RECT Field::GetRoundRect(int aX, int aY)
{
  return {
    aX == 0 ? 0 : aX-1,
    aY == 0 ? 0 : aY-1,
    aX == mSize.Width-1 ? mSize.Width-1 : aX+1,
    aY == mSize.Height-1 ? mSize.Height-1 : aY+1
  };
}

template <typename _TypeName>
int Field::CountAround(int aX, int aY, _TypeName Cell::*PField, _TypeName aValue)
{
  if(!mSize.IsPointInRect(aX, aY))
    return -1;
  int result = 0;
  RECT r = GetRoundRect(aX, aY);
  for(int i = r.left; i <= r.right; ++i)
    for(int j = r.top; j <= r.bottom; ++j)
      if(mCells[i][j].*PField == aValue && (aX != i || aY != j))
        ++result;
  return result;
}


/// class SapperGame ///

inline bool SapperGame::IsPosCorrect(ShortPoint aPos, ShortPoint aStartPos)
{
  if(aPos == aStartPos)
    return false;
  return !IsPointInRect(aPos, mField.GetRoundRect(aStartPos.x, aStartPos.y));

  //return aPos != aStartPos && !IsPointInRect(aPos, mField.GetRoundRect(aStartPos.x, aStartPos.y));
}


/// functions ///

inline bool operator==(ShortPoint aPos1, ShortPoint aPos2)
{
  return aPos1.x == aPos2.x && aPos1.y == aPos2.y;
}

inline bool operator!=(ShortPoint aPos1, ShortPoint aPos2)
{
  return !(aPos1 == aPos2);
}

inline bool IsPointInRect(ShortPoint aPoint, const RECT &aRect)
{
  return aPoint.x >= aRect.left && aPoint.x <= aRect.right &&
         aPoint.y >= aRect.top && aPoint.y <= aRect.bottom;
}

inline bool DrawRect(HDC aHdc, const RECT &aRect)
{
  return Rectangle(aHdc, aRect.left, aRect.top, aRect.right, aRect.bottom);
}

inline bool DrawEllipse(HDC aHdc, const RECT &aRect)
{
  return Ellipse(aHdc, aRect.left, aRect.top, aRect.right, aRect.bottom);
}

inline void SetDCBrushAndPenColors(HDC aHdc, COLORREF aBrushColor, COLORREF aPenColor)
{
  SetDCBrushColor(aHdc, aBrushColor);
  SetDCPenColor(aHdc, aPenColor);
}

inline bool DrawRect(HDC aHdc, const RECT &aRect, COLORREF aBrushColor, COLORREF aPenColor)
{
  SetDCBrushAndPenColors(aHdc, aBrushColor, aPenColor);
  return DrawRect(aHdc, aRect);
}

inline bool DrawEllipse(HDC aHdc, const RECT &aRect, COLORREF aBrushColor, COLORREF aPenColor)
{
  SetDCBrushAndPenColors(aHdc, aBrushColor, aPenColor);
  return DrawEllipse(aHdc, aRect);
}

inline void DrawFlag(HDC aHdc, const RECT &aRect, COLORREF aBrushColor, COLORREF aPenColor)
{
  SetDCBrushAndPenColors(aHdc, aBrushColor, aPenColor);
  return DrawFlag(aHdc, aRect);
}

inline void SetWindowClientSize(HWND aHwnd, int aWidth, int aHeight, bool aUpdateCoeffs)
{
  Size s = GetWindowSizeCoeffs(aHwnd, aUpdateCoeffs);
  SetWindowPos(aHwnd, HWND_TOP, 0, 0, aWidth + s.Width, aHeight + s.Height,
    SWP_NOMOVE | SWP_SHOWWINDOW);
}


#endif // MINESWEEPERENGINE_H_INCLUDED
