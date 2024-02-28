/*
  (c) Vitaly Smirnov [VSdev]
  mrmaybelately@gmail.com
  2023-2024
*/


//#define DEBUG_MODE


#include <windows.h>
#include <exception>

#include "minesweeper_winos.hpp"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, int nCmdShow)
{
  try {
    MinesweeperGame Game;
    Game.Run();
  } catch (const std::exception& e) {
    MessageBox(nullptr, e.what(), "Error!", MB_OK | MB_ICONERROR);
    return 1;
  } catch(const char* msg) {
    MessageBox(nullptr, msg, "Error!", MB_OK | MB_ICONERROR);
    return 2;
  } catch(...) {
    MessageBox(nullptr, "Unknown error!", "Error!", MB_OK | MB_ICONERROR);
    return 3;
  }

  return 0;
}

