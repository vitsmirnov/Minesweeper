/*
  (c) Vitaly Smirnov [VSdev]
  mrmaybelately@gmail.com
  2023-2024
*/

#include "vs_win_extra.hpp"

#include <windows.h>
//#include <windowsx.h>


namespace vslib
{

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
    //Polyline(aHdc, p, 5);//*/
  }

  RECT ScaledRect(const RECT& rect, double scale)
  {
    // It isn't smart
    scale = (scale - double(1)) / double(2);
    int w = int(double(rect.right - rect.left) * scale);
    int h = int(double(rect.bottom - rect.top) * scale);
    return {rect.left - w, rect.top - h, rect.right + w, rect.bottom + h};
  }


} // namespace vslib
