/*
  (c) Vitaly Smirnov [VSdev]
  mrmaybelately@gmail.com
  2023-2024
*/

#ifndef VS_WIN_EXTRA_HPP_INCLUDED
#define VS_WIN_EXTRA_HPP_INCLUDED


#include <windows.h>
//#include <windowsx.h>
//#include <gdiplus.h>


namespace vslib
{

  using ShortPoint = POINTS;

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

  RECT ScaledRect(const RECT& rect, double scale);


  /// IMPLEMENTATION ///

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


} // namespace vslib


#endif // VS_WIN_EXTRA_HPP_INCLUDED
