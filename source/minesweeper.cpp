/*
  (c) Vitaly Smirnov [VSdev]
  mrmaybelately@gmail.com
  2023-2024
*/


//#define DEBUG_MODE


#include <windows.h>
#include <exception>

#include "minesweeper_winos.hpp"


void ShowError(const char* msg);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, int nCmdShow)
{
  try {
    MinesweeperGame Game;
    Game.Run();
  } catch (const std::exception& e) {
    ShowError(e.what());
    return 1;
  } catch(const char* msg) {
    ShowError(msg);
    return 2;
  } catch(...) {
    ShowError("Unknown error!");
    return 3;
  }

  return 0;
}

void ShowError(const char* msg)
{
  MessageBox(nullptr, msg, "Error!", MB_OK | MB_ICONERROR);
}
