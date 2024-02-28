/*
  (c) Vitaly Smirnov [VSdev]
  mrmaybelately@gmail.com
  2023-2024
*/

#include "minesweeper_winos.hpp"

#include <cstdio>
//#include <ctime>

#include <windows.h>
#include <windowsx.h>

#include <utility>
#include <memory>

#include "vs_user_input.hpp"
#include "vs_win_keys.hpp"

// Temp!
//#include <conio.h>
//#include <iostream>
//#include <bitset>
//#include <string>


//using namespace minesweeper_game_core;
namespace mgc = minesweeper_game_core;
using namespace vslib;


/// class MinesweeperGame ///

MinesweeperGame::MinesweeperGame(const LevelData& level,
  const mgc::Size &aCellSize, CellColorSet color_set):
  _engine{level.field_size, level.mines_count},
  _controller{new MinesweeperController{this, &_engine}},
  //_main_window{new ControlledWindow{_controller->GetWindowProc(), WindowData{"", WS_OVERLAPPEDWINDOW}}}
  _color_set{color_set}
{
  WindowData wdata("Minesweeper", WS_OVERLAPPEDWINDOW);//WS_OVERLAPPED | WS_BORDER | WS_SYSMENU
  _main_window.reset(new ControlledWindow{_controller->GetWindowProc(), wdata});

  SetCellSize(aCellSize);
  //FitWindowSize();
}

void MinesweeperGame::Run()
{
  FitWindowSize();
  MoveWindowToScreenCenter();
  _main_window->ShowAs(SW_SHOWDEFAULT);
  UpdateWindow(HWnd());

#ifndef DEBUG_MODE
  MessageBox(HWnd(), MsgWelcome, MsgWelcomeCapt, MB_OK | MB_ICONINFORMATION);
  //ShowInfo();
#endif // RELEASE_MODE

  RunGameLoop();
}

void MinesweeperGame::SetCellSize(const mgc::Size &new_size)
{
  if (_cell_size == new_size)
    return;

  _cell_size = new_size;
  _cell_size.FitInto(MinCellSize, MaxCellSize);

  FitWindowSize();
  UpdateScreen(GetDC(HWnd()));//Repaint();
  //MoveWindowToScreenCenter();
}

void MinesweeperGame::RunGameLoop()
{
  if (!HWnd())
    throw "Error: MinesweeperGame::RunGameLoop() failed. HWnd is nullptr";//return;

  HDC hdc = GetDC(HWnd());
  if (!hdc)
    throw "Error: MinesweeperGame::RunGameLoop() failed. hdc is nullptr";//return;

  bool break_loop = false;
  int t = 0;
  DWORD t_prev = GetTickCount();

  constexpr DWORD frame_duration = DWORD(1000 / 60);

  do {
    /// User input
    GetAndDoPlayerCommand(break_loop);
    if (break_loop)
      break;

    /// Game logic
    t = GetTickCount() - t_prev;
    t_prev = GetTickCount();
    UpdateGameLogic(t);
    //if(BreakLoop)
      //break;

    /// Rendering
    UpdateScreen(hdc);

    // Stabilize FPS
    wait(frame_duration - (GetTickCount() - t_prev));
  } while (!break_loop);
}

void MinesweeperGame::UpdateGameLogic(int delta_time)
{
  if (_engine.IsGameOver()) {
    UpdateScreen(GetDC(HWnd()));// !!!
    char msg[100];//256
    if (_engine.GetGameState() == GameEngine::GameState::HasLost)
      sprintf(msg, MsgGameOverLost);
    else if (_engine.GetGameState() == GameEngine::GameState::HasWon)
      sprintf(msg, MsgGameOverWon, _engine.GetCurrentGameTime());
    if (MessageBox(HWnd(), msg, MsgGameOverCapt, MB_YESNO) == IDNO)
      PostQuitMessage(0);
    else
      _engine.NewGame();
  }
}

// We can do it as a class (or functor) so that we don't need to use static variables
// frequency is how often we need to update FPS (in milliseconds).
static double GetAndTickFPS(unsigned int frequency = 1000, bool do_update = true)
{
  static unsigned int frames = 0;
  static DWORD t = GetTickCount();
  static double fps = 0.0;

  if (!do_update)
    return fps;

  DWORD cur_t = GetTickCount() - t;
  if (cur_t >= frequency && cur_t != 0) {
    fps = (double)frames / (double)cur_t * (double)1000; // 1000 is needed to convert milliseconds to seconds
    frames = 0;
    t = GetTickCount();
  }

  ++frames;

  return fps;
}

const char* MinesweeperGame::GetGameStateInfo()
{
  constexpr unsigned char info_str_len = 200;
  static char info_str[info_str_len];// + 1? static ?

  sprintf_s(info_str, info_str_len, MsgGameStateInfo, _engine.GetCurrentGameTime(),
    _engine.GetMinesCount() - _engine.GetMineMarkedCount());

  if (_show_extra_info) {
    auto l = strlen(info_str);
    sprintf_s(info_str + l, info_str_len - l, MsgGameStateInfoExtra,
      GetAndTickFPS(1000, true), _engine.GetNotOpenedCellsCount());
    //strcat_s(info_str, info_str_len, MsgGameStateInfoExtra);
    //sprintf_s(info_str, info_str_len, info_str, GetFPS(1000), _engine.GetNotOpenedCellsCount()); // It's not safe
  }

  return info_str;
}

void MinesweeperGame::UpdateScreen(HDC hdc)
{
  // This definitely needs to be redone!
  //#warning Bad drawing

  //HDC hdc = GetDC(mHwnd);//static?

  mgc::Size s = GetFieldScreenSize();
  //s.height += 50;

  HDC mdc = CreateCompatibleDC(hdc);
  HBITMAP mbm = CreateCompatibleBitmap(hdc, s.width, s.height);
  SelectObject(mdc, mbm);

  // Background
  SelectObject(mdc, GetStockObject(DC_BRUSH));
  SetDCBrushColor(mdc, RGB(100, 150, 200));//?
  SelectObject(mdc, GetStockObject(DC_PEN));
  SetDCPenColor(mdc, RGB(255, 0, 0));//?
  //Rectangle(mdc, 0, 0, s.width, s.height); // /*400, 200);//*/ClientRect.right+0, ClientRect.bottom+0);
  //FillRect(mdc, &ClientRect, (HBRUSH)RGB(100, 150, 200));

  DrawGameField(mdc);

  //Scoreboard
  /*SetBkColor(hdc, _color_set.opened_cell.body);
  SetTextColor(hdc, _color_set.number);
  RECT r{0, s.height - 50, s.width, s.height};
  DrawText(mdc, GetGameStateInfo(), -1, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);//*/

  BitBlt(hdc, 0, 0, s.width, s.height, mdc, 0, 0, SRCCOPY);
  //StretchBlt(hdc, 0, 0, s.width, s.height, mdc, 50, 30, 200, 180, SRCCOPY);

  DeleteObject(mbm);
  DeleteDC(mdc);//DeleteObject(mdc);//*/


  /*ClientRect.left = 600;
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

  // Scoreboard (caption)
  _main_window->SetCaption(GetGameStateInfo());
}

static void DrawFieldCell(const Cell& cell, HDC hdc, const RECT& rect,
  int* mines_around, const CellColorSet& colors = CellColorSet{})
  // I guess mines_around shouldn't be a pointer
  // After optimization mines_around will be right in Cell struct
{
  switch (cell.status) {
    case Cell::Status::Closed:
      DrawRect(hdc, rect, colors.closed_cell);
      break;
    case Cell::Status::MineMarked:
      DrawRect(hdc, rect, colors.closed_cell);
      DrawFlag(hdc, rect, colors.flag);
      break;
    case Cell::Status::Opened:
      DrawRect(hdc, rect, colors.opened_cell);
      if (cell.has_mine) {
        DrawEllipse(hdc, ScaledRect(rect, 0.6), colors.mine);
      } else {
        if(mines_around && *mines_around > 0) {
          //SetTextAlign(hdc, TA_CENTER);
          SetBkColor(hdc, colors.opened_cell.body);
          SetTextColor(hdc, colors.number);
          char s[2];
          sprintf_s(s, 2, "%d", *mines_around);
          //TextOut(hdc, rect.left, rect.top, s, strlen(s));
          /*DRAWTEXTPARAMS dtp;
          dtp.cbSize = sizeof(dtp);
          DrawTextEx(hdc, s, -1, &r, DT_CENTER, &dtp);//*/
          RECT r = rect;
          DrawText(hdc, s, -1, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        }
      }
      break;
  }
}

void MinesweeperGame::DrawGameField(HDC hdc)
{
  const GameField& field = _engine.GetField();
  for (mgc::Point::FieldType i = 0; i < field.GetWidth(); ++i)
    for (mgc::Point::FieldType j = 0; j < field.GetHeight(); ++j) {
      // Move it from here!
      RECT r = {
        i*_cell_size.width,
        j*_cell_size.height,
        i*_cell_size.width + _cell_size.width,
        j*_cell_size.height + _cell_size.height
      };
      // After optimization mines_around will be right in Cell struct
      int ma = field(i, j).IsOpened() ? field.MinesAround({i, j}) : -1;
      DrawFieldCell(field(i, j), hdc, r, ma >= 0 ? &ma : nullptr, _color_set);
    }
}

void MinesweeperGame::ProcessMessages(bool &quit)
{
  MSG msg;

  /*// If we want to wait for messages.
  quit = GetMessage(&msg, nullptr, 0, 0) == 0;
  TranslateMessage(&msg);
  DispatchMessage(&msg);
  //return;//*/

  // check for messages
  while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) != 0) { //GetMessage(&msg, NULL, 0, 0) != 0 //WaitMessage();//?
    // handle or dispatch messages
    quit = msg.message == WM_QUIT;// || msg.message == WM_CLOSE || msg.message == WM_DESTROY;
    if(quit) {
      break;
    } else {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }
}

void MinesweeperGame::Repaint()
{
  PAINTSTRUCT ps;
  HDC hdc = BeginPaint(HWnd(), &ps);

  UpdateScreen(hdc);

  EndPaint(HWnd(), &ps);
}


/// class MinesweeperController ///

void MinesweeperController::SetPlayerCommands()
{
  using UIE = UserInputEvent;

  SetCommand({UIE::KeyDown, VK_ESCAPE, 0}, Quit);
  SetCommand({UIE::KeyDown, VK_BACK, 0}, NewGame);

  SetCommand({UIE::MouseUp, VK_LBUTTON, 0}, OpenCell);
  SetCommand({UIE::MouseUp, VK_RBUTTON, 0}, MineMarkCell);
  SetCommand({UIE::MouseUp, VK_LBUTTON, MK_RBUTTON}, OpenSurroundingCells);
  SetCommand({UIE::MouseUp, VK_RBUTTON, MK_LBUTTON}, OpenSurroundingCells);
  SetCommand({UIE::MouseUp, VK_MBUTTON, 0}, OpenSurroundingCells);

  SetCommands(UIE::KeyDown, {VK_NUMPAD1, VK_1}, 0, SetLevel1);//0x31
  SetCommands(UIE::KeyDown, {VK_NUMPAD2, VK_2}, 0, SetLevel2);//0x32
  SetCommands(UIE::KeyDown, {VK_NUMPAD3, VK_3}, 0, SetLevel3);//0x33
  SetCommands(UIE::KeyDown, {VK_D, VK_RIGHT}, 0, IncGameFieldWidth);//'D'
  SetCommands(UIE::KeyDown, {VK_A, VK_LEFT}, 0, DecGameFieldWidth);//'A'
  SetCommands(UIE::KeyDown, {VK_S, VK_DOWN}, 0, IncGameFieldHeight);//'S'
  SetCommands(UIE::KeyDown, {VK_W, VK_UP}, 0, DecGameFieldHeight);//'W'
  SetCommands(UIE::KeyDown, {VK_E, VK_0}, 0, IncMinesCount);//'E'
  SetCommands(UIE::KeyDown, {VK_Q, VK_9}, 0, DecMinesCount);//'Q'

  SetCommands(UIE::KeyDown, {VK_ADD, VK_PLUS}, 0, IncCellSize);//187
  SetCommands(UIE::KeyDown, {VK_SUBTRACT, VK_MINUS}, 0, DecCellSize);//189
  SetCommand({UIE::MouseWheel, 0, MK_CONTROL}, ChangeCellSize);

  SetCommands(UIE::KeyDown, {VK_F1, VK_H}, 0, ShowInfo);
  SetCommands(UIE::KeyDown, {VK_RETURN, VK_TAB}, 0, FitWindowSize);
  SetCommand({UIE::KeyDown, VK_TAB, 0}, MoveWindowToScreenCenter);
  SetCommands(UIE::KeyDown, {VK_F2, VK_I}, 0, SwitchShowExtraInfo);
}

void MinesweeperController::HandleUserInput(UINT msg, WPARAM wParam, LPARAM lParam)
{
  UserInput input{WinMsgToEvent(msg), 0, 0};
  using MouseData = std::pair<mgc::Point, short>; // mouse pos and mouse wheel shift
  constexpr unsigned int MaxDataSize = sizeof(MouseData);
  char data_buffer[MaxDataSize / sizeof(char)];
  void* data{nullptr};

  switch (input.event) {
    // Keys messages:
    case UserInputEvent::KeyDown:
    case UserInputEvent::KeyUp:
    case UserInputEvent::KeyPress:
      input.key = (KeyCode)wParam;
      break;
    // Mouse messages:
    case UserInputEvent::MouseDown:
    case UserInputEvent::MouseUp:
    case UserInputEvent::MouseDblClick:
      input.key = WinMsgToMouseButton(msg);
    case UserInputEvent::MouseWheel:
      // It isn't needed for MouseKeyUp/Down/DblClick, so
      // we will get mouse wheel shift later
    case UserInputEvent::MouseMove: {
      input.keys_state = KeysState(GET_KEYSTATE_WPARAM(wParam));
      mgc::Point mouse_pos{GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};

      if (input.event == UserInputEvent::MouseWheel) {
        data = new (data_buffer) MouseData{
          mouse_pos, (short)(GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA)};
      } else {
        data = new (data_buffer) mgc::Point{mouse_pos};
      }
    }
  }
  DoPlayerCommand(input, data);
#ifdef DEBUG_MODE
  printf("HandleInput(event, key, state): %i %i %i\n", (int)input.event, input.key, input.keys_state);
#endif // DEBUG_MODE
}

LRESULT MinesweeperController::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  //printf("WindowProc: msg = %d\n", msg);

  switch (msg) {
    case WM_CLOSE://WM_DESTROY://
      Quit();//PostQuitMessage(0);
      return 0;

    // All user input commands:
    case WM_KEYDOWN:
      //printf("key down: %i, %i\n", wParam, lParam);
    case WM_KEYUP:
    case WM_CHAR://?
      //printf("key press(WM_CHAR): %i, %c\n", wParam, (unsigned char)wParam);

    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_MBUTTONDOWN:

    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
    case WM_MBUTTONUP:

    case WM_LBUTTONDBLCLK:
    case WM_RBUTTONDBLCLK:
    case WM_MBUTTONDBLCLK:

    case WM_MOUSEMOVE:
      //if (wParam == MK_RBUTTON)
        //printf("WM_MOUSEMOVE (%i, %i)\n", GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
    case WM_MOUSEWHEEL:
      //if (msg == WM_MOUSEWHEEL)
        //printf("WM_MOUSEWHEEL: %i\n", GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA);
      //if (msg == WM_LBUTTONUP)
        //std::cout << std::bitset<8>(wParam) << std::endl;
      HandleUserInput(msg, wParam, lParam);
      return 0;

    case WM_PAINT:
      _game->Repaint();
      return 0;

    //case WM_SIZE:
      //_game->FitWindowSize();
      //return 0;

    /*case WM_GETMINMAXINFO: { // This needs to fix window size
      LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
      mgc::Size s = _game->GetFieldScreenSize();
      //SIZE s2 = GetWindowFrameSize()
      lpMMI->ptMinTrackSize.x = s.width;
      lpMMI->ptMinTrackSize.y = s.height;

      lpMMI->ptMaxTrackSize.x = s.width;
      lpMMI->ptMaxTrackSize.y = s.height;
    } return 0;//*/

    default:
      return DefWindowProc(hwnd, msg, wParam, lParam);
  }
}

void MinesweeperController::SetLevel(unsigned short lvl)
{
  if (lvl >= DefaultLevels.size())
    return;
  _engine->SetSettings(DefaultLevels[lvl]);
  // Repeated code from MinesweeperController::ChangeGameFieldSize!
  _game->FitWindowSize();
  _game->Repaint();
  _game->MoveWindowToScreenCenter();//?
}

void MinesweeperController::ChangeGameFieldSize(mgc::Size::FieldType mgc::Size::* field,
  signed char len)
{
  mgc::Size s = _engine->GetFieldSize();
  s.*field += len;
  _engine->SetFieldSize(s, true);
  // Repeated code from MinesweeperController::SetLevel!
  _game->FitWindowSize();
  _game->Repaint();
  _game->MoveWindowToScreenCenter();
}

