/*
  (c) Vitaly Smirnov [VSdev]
  mrmaybelately@gmail.com
  2023
*/

#include <windows.h>
#include <windowsx.h>
//#include <gdiplus.h>

#include <stdio.h>

#include <string>
#include <iostream>

#include <functional>

#include "SapperEngine.h"

//using namespace vslib;
using namespace std;

class TUnit {
private:
  const char *mName = "I'm unit.";
public:
  void SayHello() const {printf("%s\n", mName);}
};



class TGame {
private:
  TUnit mPlayer;

  class TCommand {
  public:
    void operator()() {}//{mPlayer.SayHello();}
  };

  //TCommand mCommand;
  std::function<void(void)> mCommand;// =
public:
  //TGame()
  void DoCommand() {mCommand();}
};


class Functor {
public:
  int operator()() const {
    printf("hello!\n");
    return 8;
  }
  void f1() {
    printf("f1()\n");
  }
};

class EmptyFunctor {
public:
  void operator()() const {
    ;
  }
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
  LPSTR lpCmdLine, int nCmdShow)
{
  string s1;
  s1 = "Hello!";
  cout << s1 << endl;
  s1[1] = '*';
  cout << s1 << endl;
  //#error error1
  //#warning somethings wrong!
  return 0;

  //function
  TGame game;
  game.DoCommand();


  Functor Commands[256];
  Functor *Commands2[256];
  Commands[0]();
  Commands2[0] = nullptr;
  Commands2[0]->f1();
  Commands2[0]->operator()();
  (*Commands2[0])();
  printf("%lld, %lld\n", sizeof(Commands), sizeof(Commands2));
  //for_each(, )
  return 0;

  /*string s1 = "Hello";
  //s1 = "hello " + "word!";
  s1 = s1 + "word!";
  //s1.length();
  //printf(s1);
  cout << s1 << endl;
  return 0;//*/

  try {
    SapperGameExecutor Game(hInstance, nCmdShow);
    Game.Run();
    /*DinoGameExecutor *Game = new DinoGameExecutor(hInstance, nCmdShow);
    Game->Run();
    delete Game;//*/
  } catch(const char* e) {
    printf("Error: %s\n", e);
    return 1;
  } catch(...) {
    printf("Unknown error!\n");
    return 2;
  }

  return 0;//msg.wParam;
}

