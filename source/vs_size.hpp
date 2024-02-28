/*
  (c) Vitaly Smirnov [VSdev]
  mrmaybelately@gmail.com
  2023
*/

#ifndef VS_SIZE_HPP_INCLUDED
#define VS_SIZE_HPP_INCLUDED


namespace vslib
{

  template <typename _Type>// = size_t>
  struct Size {
  public:
    using FieldType = _Type;//Bad name!
  public:
    _Type width{(_Type)0};
    _Type height{(_Type)0};
  public:
    void Setup(_Type _width, _Type _height) {width = _width; height = _height;}
    //void operator()(_Type _width, _Type _height) {Setup(_width, _height);}

    // It's not good for float!
    bool operator==(const Size &other) const
      {return width == other.width && height == other.height;}
    bool operator!=(const Size &other) const
      {return !operator==(other);}

    auto Area() const
      {return width*height;}

    bool IsPointInRect(_Type x, _Type y) const
      {return x >= (_Type)0 && (_Type)y >= 0 && (_Type)x < width && (_Type)y < height;}
    bool IsZero() const
      {return width == (_Type)0 || height == (_Type)0;}

    // Do we need this?
    const Size& FitIntoMin(const Size& min_size) {
      if (width < min_size.width) width = min_size.width;
      if (height < min_size.height) height = min_size.height;
      return *this;
    }
    const Size& FitIntoMax(const Size& max_size) {
      if (width > max_size.width) width = max_size.width;
      if (height > max_size.height) height = max_size.height;
      return *this;
    }
    const Size& FitInto(const Size& min_size, const Size& max_size)
      {FitIntoMin(min_size); FitIntoMax(max_size); return *this;}
  };

} // namespace vslib


#endif // VS_SIZE_HPP_INCLUDED
