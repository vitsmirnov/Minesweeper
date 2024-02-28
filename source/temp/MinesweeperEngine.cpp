/*
  (c) Vitaly Smirnov [VSdev]
  mrmaybelately@gmail.com
  2023
*/

#include "MinesweeperEngine.hpp"

#include <windows.h>

//#include <time.h>
//#include <ctime>

//#include <cstdio>
//#include <stdio.h>
//#include <conio.h>


//const short OneByte = 256;

//RECT ClientRect = {0, 0, 700, 500};

// ???
const COLORREF ColorWhite = RGB(255, 255, 255);
const COLORREF ColorBlack = RGB(0, 0, 0);


SapperGame *SapperGameExecutor::smGame = 0;


/// struct Cell  ///

bool Cell::Open()
{
  if(Status == csMineMarked)
    return false;
  Status = csOpened;
  return true;
}

bool Cell::MineMarking()
{
  if(Status == csOpened)
    return false;
  if(Status == csClosed)
    Status = csMineMarked;
  else // Status == csMineMarked
    Status = csClosed;
  return true;
  /*switch(Status) {
    case csOpened:
      return false;
    case csClosed:
      Status = csMineMarked;
      return true;
    case csMineMarked:
      Status = csClosed;
      return true
  }
  return false;//*/
}

void Cell::Draw(HDC aHdc, const RECT &aRect, int *aMinesAround)
{
  /*char s[10];
  sprintf(s, "(%d, %d)", i, j);
  TextOut(aHdc, r.left, r.top, s, strlen(s));
  continue;//*/
  switch(Status) {
    case csClosed:
      DrawRect(aHdc, aRect, RGB(50, 200, 100), RGB(0, 255, 0));
      break;
    case csMineMarked:
      DrawRect(aHdc, aRect, RGB(50, 200, 100), RGB(0, 255, 0));
      SetDCBrushColor(aHdc, RGB(250, 100, 50));
      DrawFlag(aHdc, aRect);
      break;
    case csOpened:
      if(IsMine) {
        DrawRect(aHdc, aRect, RGB(250, 200, 100), RGB(0, 255, 0));
        SetDCBrushColor(aHdc, RGB(50, 50, 50));
        DrawEllipse(aHdc, {aRect.left + 2, aRect.top + 2, aRect.right - 2, aRect.bottom - 2});
      } else {
        DrawRect(aHdc, aRect, RGB(250, 200, 100), RGB(0, 255, 0));

        if(aMinesAround && *aMinesAround > 0) {
          //SetTextAlign(aHdc, TA_CENTER);
          SetBkColor(aHdc, RGB(250, 200, 100));
          SetTextColor(aHdc, RGB(10, 105, 250));
          char s[10];
          //sprintf(s, "(%d, %d)", aRect.left, aRect.top);
          sprintf(s, "%d", *aMinesAround);
          //TextOut(aHdc, aRect.left, aRect.top, s, strlen(s));
          /*DRAWTEXTPARAMS dtp;
          dtp.cbSize = sizeof(dtp);
          dtp.//*/
          RECT r = aRect;
          DrawText(aHdc, s, -1, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
          //DrawTextEx(aHdc, s, -1, &r, DT_CENTER, &dtp);
        }
      }
      break;
  }
}


/// class Field ///

void Field::Destroy()
{
  if(!mCells)
    return;
  delete [] *mCells;
  delete [] mCells;
  mCells = 0;
}

void Field::Reset(bool aIsMine, CellStatus aCellStatus)
{
  for(int i = 0, a = mSize.Area(); i < a; ++i)
    mCells[0][i].Setup(aIsMine, aCellStatus);
}

bool Field::Setup(const Size &aSize, bool aIsMine, CellStatus aCellStatus)
{
  if(!aSize.IsValid())
    return false;//throw ??
  if(aSize != mSize) {
    Destroy();
    mSize = aSize;
    mCells = new Cell*[mSize.Width];
    *mCells = new Cell[mSize.Area()];
    for(int i = 1; i < mSize.Width; ++i)
      mCells[i] = mCells[i - 1] + mSize.Height;//&(mCells[0][i*mSize.Height]);
  }
  Reset(aIsMine, aCellStatus);
  return true;
}

void Field::Draw(HDC aHdc, const Size &aCellSize) {
  for(int i = 0; i < mSize.Width; ++i)
    for(int j = 0; j < mSize.Height; ++j) {
      RECT r = {i*aCellSize.Width, j*aCellSize.Height,
        i*aCellSize.Width + aCellSize.Width, j*aCellSize.Height + aCellSize.Height};
      int ma = mCells[i][j].Status == csOpened ? MinesAround(i, j) : -1;
      mCells[i][j].Draw(aHdc, r, ma >= 0 ? &ma : 0);
    }
}


/// class SapperGame ///

SapperGame::SapperGame(HINSTANCE aInstance, int aCmdShow, PWindowProc aWindowProc,
  const Size &aFieldSize, int aMinesCount, const Size &aCellSize):
    mHwnd(0),
    mCmdShow(aCmdShow),
    mField(aFieldSize, false, csClosed),
    mMinesCount(aMinesCount),
    mCellSize(aCellSize),
    mMineMarked(0),
    mCellOpened(0),
    mIsGameRunning(false)
{
  WNDCLASSEX mWcex;

  // register window class
  mWcex.cbSize = sizeof(WNDCLASSEX);
  mWcex.style = CS_OWNDC;
  mWcex.lpfnWndProc = aWindowProc;//aWindowProc != 0 ? &aWindowProc : &DefWindowProc;//&WindowProc;//&DefWindowProc;
  mWcex.cbClsExtra = 0;
  mWcex.cbWndExtra = 0;
  mWcex.hInstance = aInstance;
  mWcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  mWcex.hCursor = LoadCursor(NULL, IDC_ARROW);
  mWcex.hbrBackground = (HBRUSH)COLOR_WINDOW;//GetStockObject(BLACK_BRUSH);
  mWcex.lpszMenuName = NULL;
  mWcex.lpszClassName = "SapperMainWindow";
  mWcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

  if(!RegisterClassEx(&mWcex))
    throw "Error in DinoGame::DinoGame(..): couldn't RegisterClassEx(&mWcex)!";

  //Size s(GetWindowSizeCoeffs(GetConsoleWindow(), true));
  // create main window
  mHwnd = CreateWindowEx(
    0,
    mWcex.lpszClassName,
    "Sapper",
    WS_OVERLAPPEDWINDOW,// | WS_HSCROLL | WS_TILED,
    //WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
    //WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME,// & ~WS_MAXIMIZEBOX,
    //WS_OVERLAPPEDWINDOW | WS_VISIBLE,
    //WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME,
    //WS_VISIBL E| WS_SYSMENU | WS_MINIMIZEBOX,
    //WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
    200,//CW_USEDEFAULT,
    50,//CW_USEDEFAULT,
    //mField.GetSize().Width * mCellSize.Width + GetWindowSizeCoeffs(mHwnd).Width, //aFieldSize. //mScreenWidth + 16,//700,
    //mField.GetSize().Height * mCellSize.Height + GetWindowSizeCoeffs(mHwnd).Height, //aFieldSize //mScreenHeight + 39,//500,
    //mField.GetSize().Width * mCellSize.Width + s.Width,
    //mField.GetSize().Height * mCellSize.Height + s.Height,
    aFieldSize.Width * aCellSize.Width,
    aFieldSize.Height * aCellSize.Height,
    NULL,
    NULL,
    aInstance,
    NULL);
  if(!mHwnd)
    throw "Error in DinoGame::DinoGame(..): couldn't CreateWindowEx(..)!";

  SetWindowClientSize(mHwnd,
    aFieldSize.Width * aCellSize.Width, (aFieldSize.Height + 2) * aCellSize.Height, true);
  //SetWindowPos(mHwnd, 0, 100, 50, mCellWidth*mFieldWidth, mCellHeight*mFieldHeight, 0);
  //SetWindowLongPtr(mHwnd, GWL_STYLE, WS_POPUP);

  for(int i = 0; i < TwoBytes; ++i)
    mPlayerCommands[i] = 0;
  mPlayerCommands[VK_ESCAPE] = &Quit;
  mPlayerCommands[VK_RETURN] = &Test1;

  srand(GetTickCount());// time(NULL));
  //srand(time(NULL));
  //mField.Reset()
}

SapperGame::~SapperGame()
{
  DestroyWindow(mHwnd);// destroy the window explicitly
}

void SapperGame::Run()
{
  ShowWindow(mHwnd, mCmdShow);
  UpdateWindow(mHwnd);//?

  RunGameLoop();
}

void SapperGame::RunGameLoop()
{
  if(!mHwnd)
    return;

  HDC hdc = GetDC(mHwnd);
  if(!hdc)
    return;
  char InfoStr[256];
  unsigned int Frames = 0;
  bool BreakLoop = false;
  double FPS = 0.0;
  int t = 0;
  DWORD tPrev = GetTickCount();
  DWORD t2 = tPrev;

  do {
    // User's input
    //ProcessMessages(BreakLoop);
    GetAndDoPlayerCommand(BreakLoop);
    if(BreakLoop)
      break;

    // Game logic
    t = GetTickCount() - tPrev;
    tPrev = GetTickCount();
    UpdateGameLogic(t);

    // Rendering
    UpdateScreen(hdc);

    // Info
    if(GetTickCount() - t2 >= 1000) {
      FPS = (double)Frames / (double)(GetTickCount() - t2) * 1000.0;
      Frames = 0;
      t2 = GetTickCount();
    }
    sprintf(InfoStr, "Mines: %i, Mines left: %i, Cell left: %i, FPS: %.0f",
      mMinesCount, mMinesCount - mMineMarked, mField.GetSize().Area() - mCellOpened, FPS);
    //sprintf(InfoStr, "Time: %ld, FPS: %.0f, frames: %d", GetTickCount() / 1000, FPS, Frames);
    SetWindowText(mHwnd, InfoStr);
    ++Frames;
    if(Frames % 5 == 0)
      Sleep(1);
  } while(!BreakLoop);
}

void SapperGame::UpdateGameLogic(int aDeltaTime)
{
  ;
}

void SapperGame::UpdateScreen(HDC aDc)
{
  //HDC aDc = GetDC(mHwnd);

  //printf("UpdateScreen: (%ld, %ld, %ld, %ld)\n",
    //ClientRect.left, ClientRect.top, ClientRect.right, ClientRect.bottom);
  //int width = 600;
  //int height = 400;
  //RECT CRC = ClientRect;
  //ClientRect.right = mScreenWidth;//600;
  //ClientRect.bottom = mScreenHeight;//400;
  Size s = GetFieldScreenSize();

  HDC mdc = CreateCompatibleDC(aDc);
  HBITMAP mbm = CreateCompatibleBitmap(aDc, s.Width, s.Height);//ClientRect.right, ClientRect.bottom);
  SelectObject(mdc, mbm);

  // Background
  SelectObject(mdc, GetStockObject(DC_BRUSH));
  SetDCBrushColor(mdc, RGB(100, 150, 200));
  SelectObject(mdc, GetStockObject(DC_PEN));
  SetDCPenColor(mdc, RGB(255, 0, 0));
  Rectangle(mdc, 0, 0, s.Width, s.Height);// /*400, 200);//*/ClientRect.right+0, ClientRect.bottom+0);
  //FillRect(mdc, &ClientRect, (HBRUSH)RGB(100, 150, 200));

  // Objects
  /*SelectObject(mdc, GetStockObject(DC_BRUSH));
  SetDCBrushColor(mdc, RGB(50, 200, 100));
  SelectObject(mdc, GetStockObject(DC_PEN));
  SetDCPenColor(mdc, RGB(0, 255, 0));
  //Rectangle(mdc, 70, 40, 200, 150);//*/
  //Rectangle(mdc, (int)Dino.mPosition.X, (int)Dino.mPosition.Y,
    //(int)Dino.mPosition.X + Dino.mWidth, (int)Dino.mPosition.Y + Dino.mHeight);
  mField.Draw(mdc, mCellSize);

  //BitBlt(aDc, 0, 0, ClientRect.right, ClientRect.bottom, mdc, 0, 0, SRCCOPY);
  BitBlt(aDc, 0, 0, s.Width, s.Height, mdc, 0, 0, SRCCOPY);

  DeleteObject(mbm);
  DeleteDC(mdc);//DeleteObject(mdc);//*/

  /*int kc = RGB(0, 150, 200);
  RECT CRC = ClientRect;//Client rect copy
  ClientRect.left = 600;
  ClientRect.bottom = 400;

  //создаем совместимый виртуальный хэндл контекста
  HDC mdc = CreateCompatibleDC(dc);
  //создаем и устанавливаем карту памяти
  HBITMAP mbm = CreateCompatibleBitmap(dc, ClientRect.right, ClientRect.bottom);
  SelectObject(mdc, mbm);
  //дублируем прежнее изображение нашего окна
  //BitBlt(mdc, 0, 0, ClientRect.right, ClientRect.bottom, dc, 0, 0, SRCCOPY);
  //Rectangle(mdc, 0, 0, WindowSize.x - 10, WindowSize.y - 10);
  HBRUSH brush = CreateSolidBrush(kc);
  SelectObject(mdc, brush);
  //RECT r = ClientRect;
  //r.right -= 10;
  FillRect(mdc, &ClientRect, brush);//(HBRUSH)COLOR_WINDOW);

  //рисуем квадратик
  SelectObject(mdc, brush);
  Rectangle(mdc, 50, 10, 200, 150);

  //копируем полученное изображение в наше окно
  //BitBlt(dc, 0, 0, width, height, mdc, 0, 0, SRCCOPY);
  BitBlt(dc, 0, 0, ClientRect.right, ClientRect.bottom, mdc, 0, 0, SRCCOPY);

  //освобождаем память
  DeleteObject(brush);
  DeleteObject(mbm);
  DeleteDC(mdc);//DeleteObject(mdc);//*/

  //ClientRect = CRC;
}

void SapperGame::ProcessMessages(bool &aQuit)
{
  MSG msg;
  // check for messages
  while(PeekMessage(&msg, /*mHwnd*/NULL, 0, 0, PM_REMOVE) > 0) { //GetMessage(&msg, NULL, 0, 0)
    //printf("ProcessMessages: msg = %d\n", msg.message);
    // handle or dispatch messages
    // I guess, that messages like WM_QUIT, WM_CLOSE etc don't appear here. No!
    aQuit = msg.message == WM_QUIT;// || msg.message == WM_CLOSE || msg.message == WM_DESTROY;
    if(!aQuit) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    } else {
      //break;//PeekMessage probably(!) returns 0 if the message == WM_QUIT (??)
    }
  }//*/

  /*// check for messages
  while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0) {
    // handle or dispatch messages
    if (msg.message == WM_QUIT) {
      bQuit = TRUE;
    } else {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }//*/
}

inline int MsgToKey(UINT aMsg, WPARAM aWParam)
{
  switch(aMsg) {
    case WM_KEYDOWN:
      return aWParam;
    case WM_KEYUP:
      return aWParam + OneByte;
    case WM_LBUTTONDOWN:
      return VK_LBUTTON;
    case WM_RBUTTONDOWN:
      return VK_RBUTTON;
    case WM_MBUTTONDOWN:
      return VK_MBUTTON;
    case WM_LBUTTONUP:
      return VK_LBUTTON + OneByte;
    case WM_RBUTTONUP:
      return VK_RBUTTON + OneByte;
    case WM_MBUTTONUP:
      return VK_MBUTTON + OneByte;
    default:
      return 0;
  }
}

//LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
LRESULT SapperGame::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  //printf("WindowProc: msg = %d\n", uMsg);

  switch (uMsg) {
    case WM_CLOSE:
      PostQuitMessage(0);
      break;

    //case WM_DESTROY:
      //return 0;

    case WM_KEYDOWN:
      if(wParam == VK_BACK) {
        mMineMarked = 0;
        mCellOpened = 0;
        PutMines({1, 1}, mMinesCount);// 20);
      }
      //break;
    case WM_KEYUP:
    //case WM_LBUTTONDOWN:
    //case WM_RBUTTONDOWN:
    //case WM_MBUTTONDOWN:
    /*case WM_LBUTTONUP:
    case WM_RBUTTONUP:
    case WM_MBUTTONUP://*/
      DoPlayerCommand(MsgToKey(uMsg, wParam));
      //Player.SetPosFromClient(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
      break;

    /*case WM_LBUTTONUP:
      printf("WM_LBUTTONDOWN, wParam = %llu, (%s%s%s%s%s%s)\n", wParam,
             wParam & MK_LBUTTON ? "LB " : "",
             wParam & MK_RBUTTON ? "RB " : "",
             wParam & MK_MBUTTON ? "MB " : "",
             wParam & MK_CONTROL ? "Ctrl " : "",
             wParam & MK_SHIFT ? "Shift " : "",
             wParam & MK_ALT ? "Alt " : "");
      OpenCell(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
      break;
    case WM_RBUTTONUP:
      MineMarkedOrClose(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
      break;//*/
    /*case WM_LBUTTONDBLCLK:
      printf("WM_LBUTTONDBLCLK\n");
      OpenAround(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
      break;//*/
    case WM_LBUTTONUP:
      if(wParam & MK_RBUTTON)
        OpenAround(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
      else
        OpenCell(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
      break;
    case WM_RBUTTONUP:
      if(wParam & MK_LBUTTON)
        OpenAround(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
      else
        MineMarkedOrClose(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
      break;

    case WM_MOUSEMOVE:
      if(wParam == MK_RBUTTON)
        ;//mPlayer->SetPosFromClient(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
      break;//*/

    case WM_PAINT:
      Repaint(hwnd);
      break;

    case WM_SIZE:
      //RECT r;
      //GetClientRect(hwnd, &r);
      //WindowSize = {r.right, r.bottom};
      //GetClientRect(hwnd, &ClientRect);
      //printf("WM_SIZE (%ld, %ld)\n", ClientRect.right, ClientRect.bottom);
      printf("WM_SIZE\n");
      break;

    /*case WM_GETMINMAXINFO: {
        LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
        lpMMI->ptMinTrackSize.x = 500;
        lpMMI->ptMinTrackSize.y = 400;

        lpMMI->ptMaxTrackSize.x = 500;
        lpMMI->ptMaxTrackSize.y = 400;
      }
      break;//*/

    default:
      return DefWindowProc(hwnd, uMsg, wParam, lParam);
  }

  return 0;
}//*/

void SapperGame::Repaint(HWND aHwnd)
{
  static unsigned long long n = 0;
  ++n;
  printf("WM_PAINT (%lld)!\n", n);

  PAINTSTRUCT ps;
  HDC hdc = BeginPaint(aHwnd, &ps);

  UpdateScreen(hdc);

  EndPaint(aHwnd, &ps);
}

bool SapperGame::PutMines(ShortPoint aStartPos, int aMineCount)
{
  if(!mField.GetSize().IsPointInRect(aStartPos) || !IsMineCountValid(aMineCount, mField.GetSize()))
    return false;
  mField.Reset(false, csClosed);
  for(int l = mField.GetSize().Area(); aMineCount > 0; ) {
    int n = vslib::Random(l-1);
    if(!mField[n].IsMine && IsPosCorrect(PosToPoint(n), aStartPos)) {
      mField[n].IsMine = true;
      --aMineCount;
      //printf("Put mine at (%d, %d), mc = %d\n", PosToPoint(n).x, PosToPoint(n).y, mc);
    }
  }
  return true;
}

void SapperGame::OpenCell(int aX, int aY)
{
  if(!mField.GetSize().IsPointInRect(aX /= mCellSize.Width, aY /= mCellSize.Height))
    return;
  if(!mField(aX, aY).Open() || mField(aX, aY).IsMine)
    return;
  ++mCellOpened;
  if(mField.MinesAround(aX, aY) > 0)// || mField.CountAround<>(aX, aY));
    return;
  // Double code! Another -> OpenAround
  RECT r = mField.GetRoundRect(aX, aY);
  for(int i = r.left; i <= r.right; ++i)
    for(int j = r.top; j <= r.bottom; ++j)
      if(mField(i, j).Status == csClosed && (i != aX || j != aY))
        OpenCell(i * mCellSize.Width, j * mCellSize.Height);
}

void SapperGame::MineMarkedOrClose(int aX, int aY)
{
  if(!mField.GetSize().IsPointInRect(aX /= mCellSize.Width, aY /= mCellSize.Height))
    return;
  //if(mField(aX, aY).Status == csOpened)//.Open())// || mField(aX, aY).IsMine)
    //return;
  if (mField(aX, aY).MineMarking())
    mMineMarked += mField(aX, aY).Status == csMineMarked ? 1 : -1;
}

void SapperGame::OpenAround(int aX, int aY)
{
  if(mField(aX /= mCellSize.Width, aY /= mCellSize.Height).Status != csOpened || mField(aX, aY).IsMine)
    return;
  if(mField.MinesAround(aX, aY) != mField.MineMarkedAround(aX, aY) ||
     mField.CountAround<CellStatus>(aX, aY, &Cell::Status, csClosed) == 0)
  {
    return;
  }
  RECT r = mField.GetRoundRect(aX, aY);
  for(int i = r.left; i <= r.right; ++i)
    for(int j = r.top; j <= r.bottom; ++j)
      if(mField(i, j).Status == csClosed && (i != aX || j != aY))
        OpenCell(i * mCellSize.Width, j * mCellSize.Height);
}

void SapperGame::StartGame(ShortPoint aPos)
{
  PutMines(aPos, 10);
  //Start time count
  //..
}


/// other functions ///

/*inline bool AreRectsColliding(const RECT &aRect1, const RECT &aRect2)
{
  return !(aRect1.right < aRect2.left || aRect1.left > aRect2.right ||
           aRect1.bottom < aRect2.top || aRect1.top > aRect2.bottom);
}//*/

void DrawFlag(HDC aHdc, const RECT &aRect)
{
  int w = aRect.right - aRect.left;
  int h = aRect.bottom - aRect.top;
  int w10 = (int)((double)w * 0.1);
  int h10 = (int)((double)h * 0.1);
  POINT p[5] = {
    {aRect.left + w10, aRect.bottom - h10},
    {aRect.left + w10, aRect.top + h10},
    {aRect.right - w10, aRect.top + h10*3},
    {aRect.left + w10*2, aRect.top + h10*5},
    {aRect.left + w10*2, aRect.bottom - h10}
  };
  Polygon(aHdc, p, 5);
  //Polyline(aHdc, p, 5);
}

const Size& GetWindowSizeCoeffs(HWND aHwnd, bool aUpdateCoeffs)
{
  static Size Coeffs(-1, -1);
  if(aUpdateCoeffs || (Coeffs.Width < 0 || Coeffs.Height < 0)) {
    RECT r;
    GetWindowRect(aHwnd, &r);
    //printf("GetWindowRect = (%d, %d, %d, %d)\n", r.left, r.top, r.right, r.bottom);
    Coeffs.Width = r.right - r.left;
    Coeffs.Height = r.bottom - r.top;
    GetClientRect(aHwnd, &r);
    //printf("GetClientRect = (%d, %d, %d, %d)\n", r.left, r.top, r.right, r.bottom);
    Coeffs.Width -= r.right - r.left;
    Coeffs.Height -= r.bottom - r.top;
  }
  return Coeffs;
}


/// temp ///

void Draw1(HDC aHdc)
{
  //SelectObject(aHdc, GetStockObject(DC_BRUSH));
  SetDCBrushColor(aHdc, RGB(50, 200, 100));
  //SelectObject(aHdc, GetStockObject(DC_PEN));
  SetDCPenColor(aHdc, RGB(0, 255, 0));
  DrawRect(aHdc, {5, 10, 50, 40});
  //DrawEllipse(aHdc, GetRect());;
}

void Draw2(HDC aHdc)
{
  /*HBITMAP hBmp = (HBITMAP)LoadImage(NULL,
    //"C:\\Projects\CodeBlocks\Dino 4\resource\Dino1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    "..\\..\\resource\\Dino1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    //"1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    //"C:\1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  //printf("%s, %d\n", hBmp == NULL ? "NULL!" : "NOT NULL!", GetLastError());
  // создание memory DC для этого bitmap-а:
  HDC bmpDC = CreateCompatibleDC(aHdc);
  SelectObject(bmpDC, hBmp);
  // вывод (aHdc - HDC окна):
  BitBlt(aHdc, 0, 0, 91, 98, bmpDC, 0, 0, SRCCOPY);
  // освобождение ресурсов:
  DeleteDC(bmpDC);
  DeleteObject(hBmp);
  return;//*/

  //SelectObject(aHdc, GetStockObject(DC_BRUSH));
  SetDCBrushColor(aHdc, RGB(50, 200, 100));
  //SelectObject(aHdc, GetStockObject(DC_PEN));
  SetDCPenColor(aHdc, RGB(0, 255, 0));
  //Rectangle(aHdc, (int)mPosition.X, (int)mPosition.Y,
    //(int)mPosition.X + mWidth, (int)mPosition.Y + mHeight);
  //DrawRect(aHdc, GetRect());
  DrawEllipse(aHdc, {15, 5, 60, 50});
}

