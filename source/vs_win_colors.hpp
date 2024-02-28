#ifndef VS_WIN_COLORS_HPP_INCLUDED
#define VS_WIN_COLORS_HPP_INCLUDED


#include <wingdi.h>

//namespace vslib
// These (probably) don't need to be in namespace (?)

constexpr COLORREF clBlack = RGB(0, 0, 0); // use auto?
constexpr COLORREF clWhite = RGB(255, 255, 255);

constexpr COLORREF clRed = RGB(255, 0, 0);
constexpr COLORREF clBrightGreen = RGB(0, 255, 0);
constexpr COLORREF clBlue = RGB(0, 0, 255);

constexpr COLORREF clGreen = RGB(50, 200, 100);
constexpr COLORREF clLightRed = RGB(250, 100, 50);
constexpr COLORREF clLightBlue = RGB(0, 155, 255);

constexpr COLORREF clOrange = RGB(250, 200, 100);
constexpr COLORREF clDarkGray = RGB(50, 50, 50);
constexpr COLORREF clGray = RGB(80, 80, 80);


/*
using Color = COLORREF;

enum class Colors: Color {
  LightGreen = RGB(50, 200, 100),
  Green = RGB(0, 255, 0),
  Orange = RGB(250, 200, 100),
  LightRed = RGB(250, 100, 50),
  LightBlue = RGB(0, 155, 255),
  DarkGray = RGB(50, 50, 50)
};//*/


#endif // VS_WIN_COLORS_HPP_INCLUDED
