/*
  (c) Vitaly Smirnov [VSdev]
  mrmaybelately@gmail.com
  2023
*/

#ifndef VS_RECT_HPP_INCLUDED
#define VS_RECT_HPP_INCLUDED


namespace vslib
{

  template <typename _Type>
  struct Rect {
  public:
    using FieldType = _Type;
  public:
    _Type left{(_Type)0};
    _Type top{(_Type)0};
    _Type right{(_Type)0};
    _Type bottom{(_Type)0};
  public:
    void Setup(_Type _left, _Type _top, _Type _right, _Type _bottom)
      {left = _left; top = _top; right = _right; bottom = _bottom;}
    //void operator()(_Type _left, _Type _top, _Type _right, _Type _bottom)
      //{Setup(_left, _top, _right, _bottom);}

    bool operator==(const Rect &r) const
      {return left == r.left && top == r.top && right == r.right && bottom == r.bottom;}
    bool operator!=(const Rect &r) const
      {return !operator==(r);}

    template <typename _T>
    bool IsPointInRect(_T x, _T y) const
      {return x >= left && x <= right && y >= top && y <= bottom;}
    template <typename _T>
    bool IsPointInRect(const _T& p) const
      {return p.x >= left && p.x <= right && p.y >= top && p.y <= bottom;}
  };

}


#endif // VS_RECT_HPP_INCLUDED
