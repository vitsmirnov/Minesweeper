/*
  (c) Vitaly Smirnov [VSdev]
  mrmaybelately@gmail.com
  2023-2024
*/

#ifndef MINESWEEPER_WINOS_H_INCLUDED
#define MINESWEEPER_WINOS_H_INCLUDED


#include <windows.h>
//#include <windowsx.h>
//#include <gdiplus.h>

//#include <stdio.h>//temp

#include <memory>
#include <map>
#include <functional>
#include <utility>

#include <stdexcept>
#include <exception>

#include "minesweeper_core.hpp"
#include "minesweeper_winos_colors.hpp"

#include "vs_win_window.hpp"
#include "vs_win_extra.hpp"
//#include "vs_win_colors.hpp"
#include "vs_commands.hpp"
#include "vs_method_ptr.hpp"
#include "vs_user_input.hpp"


using namespace minesweeper_game_core;


/// classes list:
class MinesweeperGame;
class MinesweeperController;


/// class MinesweeperGame ///

// This class should be split!
class MinesweeperGame { // GameApp? // GameView?
public:
  // Can we put these in *.inc (or *.hpp) file and then #include them here?
  static constexpr char MsgWelcomeCapt[] = "Welcome!"; // Capt = Caption
  static constexpr char MsgWelcome[] = "Hello! Press F1 for info.";
  static constexpr char MsgQuitCapt[] = "Quit";
  static constexpr char MsgQuit[] = "Quit?";
  static constexpr char MsgGameOverCapt[] = "Game over";
  static constexpr char MsgGameOverWon[] = "You won. Your time: %i seconds. Play again?";
  static constexpr char MsgGameOverLost[] = "You died. Play again?";
  static constexpr char MsgInfoCapt[] = "Info";
  static constexpr char MsgInfo[] =
    "Control keys:\n"
    " - open cell\t\tLeft mouse button\n"
    " - put on/off flag\t\tRight mouse button\n"
    " - open surrounding cells\tBoth/mid mouse buttons\n"
    "\n"
    " - new game\t\tBackspase\n"
    " - change level\t\t1, 2, 3\n"
    //"Customize level:\n"
    " - inc game field width\tD, Right\n"
    " - dec game field width\tA, Left\n"
    " - inc game field height\tS, Down\n"
    " - dec game field height\tW, Up\n"
    " - inc mines count\t\tE, 0\n"
    " - dec mines count\t\tQ, 9\n"
    "\n"//"Customize view:\n"//"Set up the view:\n"
    " - + / - window size\t+ / - / Ctrl+mouse wheel\n"
    " - move window to center\tTab\n"
    " - fit window size\t\tEnter\n"
    " - show/hide extra info\tF2, I\n"
    " - info/about\t\tF1, H\n"
    "\n\n"
    "About:\n"
    "   (c) Vitaly Smirnov [VSdev]\n"
    "   mrmaybelately@gmail.com\n"
    "   2023-2024";

  static constexpr char MsgGameStateInfo[] = "Time: %i, Mines left: %i";
  static constexpr char MsgGameStateInfoExtra[] = ", FPS: %.0f, Cells left: %i";

  // These shouldn't be here!
  static constexpr Size MinCellSize{20, 20}; // Pixels
  static constexpr Size MaxCellSize{100, 100}; // Pixels
  static constexpr Size DefaultCellSize{30, 30}; // Pixels
  static constexpr unsigned char CellSizeChangeStep{1}; // Pixels
public:
  MinesweeperGame(const LevelData& level = DefaultLevels[0],
    const Size &aCellSize = DefaultCellSize, CellColorSet color_set = CellColorSet{});
  MinesweeperGame(const MinesweeperGame& other) = delete;
  void operator=(const MinesweeperGame& other) = delete;
  //virtual ~SapperGame();

  void Run();

  void Quit() {
    if (MessageBox(HWnd(), MsgQuit, MsgQuitCapt, MB_YESNO | MB_ICONQUESTION) == IDYES)
      PostQuitMessage(0);
  }
  void ShowInfo() const
    {MessageBox(HWnd(), MsgInfo, MsgInfoCapt, MB_OK | MB_ICONINFORMATION);}
  void FitWindowSize() // Fit to game field
    {SetWindowClientSize(GetFieldScreenSize());}
  void MoveWindowToScreenCenter() {_main_window->SetPositionToCenter();}
  bool SwitchShowExtraInfo()
    {_show_extra_info = !_show_extra_info; return _show_extra_info;}

  const Size& GetCellSize() const {return _cell_size;}
  void SetCellSize(const Size &new_size);
  void IncCellSize() {ScaleCellSize(1, CellSizeChangeStep);}
  void DecCellSize() {ScaleCellSize(-1, CellSizeChangeStep);}

  void ScaleCellSize(short value, unsigned char step = CellSizeChangeStep) {
    SetCellSize({
      Size::FieldType(GetCellSize().width + value * step),
      Size::FieldType(GetCellSize().height + value * step)});
  }

  void SetWindowClientSize(const Size& new_size)
    {_main_window->SetClientSize(new_size.width, new_size.height);}
  auto GetFieldWidth() const {return _engine.GetFieldSize().width;}
  auto GetFieldHeight() const {return _engine.GetFieldSize().height;}
  Size GetFieldScreenSize() const {
    return {
      Size::FieldType(GetFieldWidth() * GetCellSize().width),
      Size::FieldType(GetFieldHeight() * GetCellSize().height)};
  }

  void Repaint();
protected:
  void RunGameLoop();

  void GetAndDoPlayerCommand(bool& break_game_loop)
    {ProcessMessages(break_game_loop);}
  void UpdateGameLogic(int delta_time);//, bool& break_loop);
  void UpdateScreen(HDC hdc);

  void ProcessMessages(bool& quit); // Move to vs_win_window!?
  const char* GetGameStateInfo();

  //void DrawFieldCell(const FieldCell& cell, HDC aHdc, const RECT &aRect, int *aMinesAround);
  void DrawGameField(HDC hdc);

  const HWND HWnd() const {return _main_window->GetHandle();}
private:
  GameEngine _engine;//std::unique_ptr<GameEngine>
  std::unique_ptr<vslib::WinController> _controller{nullptr};
  std::unique_ptr<vslib::Window> _main_window{nullptr};

  bool _show_extra_info{false};

  Size _cell_size{DefaultCellSize};
  CellColorSet _color_set;
};


/// class MinesweeperController ///

// There are no checks for _game or _engine is nullptr!
class MinesweeperController: public vslib::WinController { //GameController
public:
  MinesweeperController(MinesweeperGame* game, GameEngine* engine)
    : _game{game}, _engine{engine}
  {
    if (!game || !engine)
      throw "MinesweeperController init failed. game and/or engine is nullptr";
    SetPlayerCommands();
  }

  virtual vslib::WindowProcFunc GetWindowProc() override
    {return vslib::MakeMethodAsFunc(this, WindowProc);}
protected:
  void SetPlayerCommands(); // rename to SetUserControls?

  LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);//LRESULT CALLBACK ??
  void HandleUserInput(UINT msg, WPARAM wParam, LPARAM lParam);

  void DoPlayerCommand(vslib::UserInput input, void* data = nullptr) {
    //_player_commands.DoCommand(code);
    auto f = _player_commands.Find(input);
    if (f) (*f)(data);
  }
protected:// Player's commands
  // Common commands
  void Quit(void* = nullptr) {_game->Quit();}
  void ShowInfo(void* = nullptr) {_game->ShowInfo();}
  void SwitchShowExtraInfo(void* = nullptr) {_game->SwitchShowExtraInfo();}

  // Engine/game commands
  void NewGame(void* = nullptr) {_engine->NewGame();}
  void OpenCell(void* pos)
    {if (pos) _engine->OpenCell(WindowCoordsToField(*((Point*)pos)));}
  void MineMarkCell(void* pos)
    {if (pos) _engine->MineMarkCell(WindowCoordsToField(*((Point*)pos)));}
  void OpenSurroundingCells(void* pos)
    {if (pos) _engine->OpenSurroundingCells(WindowCoordsToField(*((Point*)pos)));}
  void SetLevel1(void* = nullptr) {SetLevel(0);}
  void SetLevel2(void* = nullptr) {SetLevel(1);}
  void SetLevel3(void* = nullptr) {SetLevel(2);}
  // Customize level
  void IncGameFieldWidth(void* = nullptr) {ChangeGameFieldSize(&Size::width, 1);}
  void DecGameFieldWidth(void* = nullptr) {ChangeGameFieldSize(&Size::width, -1);}
  void IncGameFieldHeight(void* = nullptr) {ChangeGameFieldSize(&Size::height, 1);}
  void DecGameFieldHeight(void* = nullptr) {ChangeGameFieldSize(&Size::height, -1);}
  void IncMinesCount(void* = nullptr)
    {_engine->SetMinesCount(_engine->GetMinesCount() + 1);}
  void DecMinesCount(void* = nullptr)
    {_engine->SetMinesCount(_engine->GetMinesCount() - 1);}

  // View/window commands
  void FitWindowSize(void* = nullptr) {_game->FitWindowSize();}
  void MoveWindowToScreenCenter(void* = nullptr) {_game->MoveWindowToScreenCenter();}
  void IncCellSize(void* = nullptr) {_game->IncCellSize();}
  void DecCellSize(void* = nullptr) {_game->DecCellSize();}
  //using MouseData = std::pair<Point, short>;
  void ChangeCellSize(void* data) // By mouse wheel
    {if (data) _game->ScaleCellSize(((std::pair<Point, short>*)data)->second);}

  // Auxiliary functions for user's commands
  void SetLevel(unsigned short lvl);
  void ChangeGameFieldSize(Size::FieldType Size::* field, signed char len);

  Point WindowCoordsToField(const Point& pos) const {
    return {
      Point::FieldType(pos.x / _game->GetCellSize().width),//+field.left
      Point::FieldType(pos.y / _game->GetCellSize().height)};
  }
protected:
  using ThisMethod = void (MinesweeperController::*)(void*);
  void SetCommand(const vslib::UserInput& input, ThisMethod f)
    {_player_commands.SetCommand(input, vslib::MakeMethod(this, f));}
  void SetCommands(vslib::UserInputEvent event, std::initializer_list<vslib::KeyCode> keys,
    vslib::KeysState state, ThisMethod f)
    {for (auto key: keys) SetCommand({event, key, state}, f);}
protected:
  using PlayerCommands =
    vslib::Commands<std::function, std::map, vslib::UserInput, void (void*)>;
private:
  PlayerCommands _player_commands;
  MinesweeperGame* _game{nullptr};//Should it be a reference instead of pointer?
  GameEngine* _engine{nullptr};//Should it be a reference instead of pointer?
};


/// class MinesweeperApp ///

/*class MinesweeperApp { // Facade
public:
  ;
private:
  ;
};//*/


/// functions ///

inline void wait(int ms)
{
  if (ms > 0)
    Sleep(ms);
}

inline bool DrawRect(HDC hdc, const RECT &rect, const FigureColor& color)
{
  return vslib::DrawRect(hdc, rect, color.body, color.frame);
}

inline bool DrawEllipse(HDC hdc, const RECT &rect, const FigureColor& color)
{
  return vslib::DrawEllipse(hdc, rect, color.body, color.frame);
}

inline void DrawFlag(HDC hdc, const RECT &rect, const FigureColor& color)
{
  return vslib::DrawFlag(hdc, rect, color.body, color.frame);
}


#endif // MINESWEEPER_WINOS_H_INCLUDED
