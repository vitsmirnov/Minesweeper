/*
  (c) Vitaly Smirnov [VSdev]
  mrmaybelately@gmail.com
  2023
*/

#ifndef VS_POINT_HPP_INCLUDED
#define VS_POINT_HPP_INCLUDED


namespace vslib
{

  template <typename _Type>
  struct Point {
  public:
    using FieldType = _Type; // Bad name!
  public:
    _Type x{(_Type)0};
    _Type y{(_Type)0};
  public:
    void Setup(_Type _x, _Type _y) {x = _x; y = _y;}
    //void operator()(_Type _x, _Type _y) {Setup(_x, _y);}

    // This is not good for float!
    bool operator==(const Point& other) const
      {return x == other.x && y == other.y;}
    bool operator!=(const Point& other) const
      {return !operator==(other);}

    bool IsZero() const
      {return x == (_Type)0 && y == (_Type)0;}

    template <typename _T>
    bool IsPointInRect(_T left, _T top, _T right, _T bottom) const
      {return x >= left && y >= top && x <= right && y <= bottom;}
  };

}


#endif // VS_POINT_HPP_INCLUDED
