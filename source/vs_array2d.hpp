/*
  (c) Vitaly Smirnov [VSdev]
  mrmaybelately@gmail.com
  2023
*/

#ifndef VS_ARRAY2D_HPP_INCLUDED
#define VS_ARRAY2D_HPP_INCLUDED


#include <iostream>

#include "vs_size.hpp"
//#include "vs_point.hpp"


namespace vslib
{

  template <typename _Type, typename _SizeType = unsigned int>
  class Array2d {
  public:
    using ArraySize = Size<_SizeType>;
  public:
    Array2d() = default;
    explicit Array2d(const ArraySize &size)
      {SetSize(size);}
    Array2d(const ArraySize &size, const _Type& default_value)
      {Setup(size, default_value);}
    virtual ~Array2d()
      {Clear();}
    // To do:
    Array2d(const Array2d& other) = delete;
    const Array2d& operator=(const Array2d& other) = delete;

    void Clear();
    bool SetSize(_SizeType width, _SizeType height)
      {return SetSize({width, height});}
    bool SetSize(const ArraySize &size);
    void Fill(const _Type& value)
      {for (size_t i = 0, a = _size.Area(); i < a; ++i) (*_values)[i] = value;}
    bool Setup(const ArraySize &size, const _Type& fill_value) {
      if (!SetSize(size))
        return false;
      Fill(fill_value);
      return true;
    }

    _SizeType GetWidth() const
      {return _size.width;}
    _SizeType GetHeight() const
      {return _size.height;}
    const ArraySize& GetSize() const
      {return _size;}
    auto GetArea() const //_SizeType // rename to GetLength?
      {return _size.Area();}

    inline _Type& At(_SizeType x, _SizeType y);
    inline const _Type& At(_SizeType x, _SizeType y) const;

    // Uses as for one-dimension array
    _Type& operator[](_SizeType index) //size_t?
      {return (*_values)[index];}
    _Type& operator()(_SizeType x, _SizeType y)
      {return _values[x][y];}

    // Uses as for one-dimension array
    const _Type& operator[](_SizeType index) const//size_t?
      {return (*_values)[index];}
    const _Type& operator()(_SizeType x, _SizeType y) const
      {return _values[x][y];}

    bool DoesPosExist(_SizeType x, _SizeType y) const
      {return x >= 0 && x < _size.width && y >= 0 && y < _size.height;}
    template <typename _T>
    bool DoesPosExist(const Point<_T>& p) const
      {return DoesPosExists(p.x, p.y);}

    void Print(const char *separator = " ", const char *caption = "",
      const char *end_line = "\n", const char *end_caption = "");
  private:
    _Type **_values{nullptr};
    ArraySize _size{0, 0};
  };


  template <typename _Type, typename _SizeType>
  void Array2d<_Type, _SizeType>::Clear()
  {
    if (!_values)//|| _size.IsZero())
      return;
    delete [] *_values;
    delete [] _values;
    _values = nullptr;
    _size.Setup((_SizeType)0, (_SizeType)0);
  }

  template <typename _Type, typename _SizeType>
  bool Array2d<_Type, _SizeType>::SetSize(const ArraySize &size)
  {
    if (_size.width < 0 || _size.height < 0)
      return false;//throw ??
    if (size != _size) {
      Clear();
      if (size.IsZero())
        return true;
      _size = size;
      try {// It's not good
        _values = new _Type*[_size.width];
        *_values = new _Type[_size.Area()];
      } catch (...) {
        return false;
      }
      for (size_t i = 1; i < _size.width; ++i)
        _values[i] = _values[i - 1] + _size.height;//&(mCells[0][i*mSize.Height]);
    }
    return true;
  }

  template <typename _Type, typename _SizeType>
  inline _Type& Array2d<_Type, _SizeType>::At(_SizeType x, _SizeType y)
  {
    if(!_size.IsPointInRect(x, y))
      throw "Field.operator(int, int): range check error!";
    return _values[x][y];
  }

  template <typename _Type, typename _SizeType>
  inline const _Type& Array2d<_Type, _SizeType>::At(_SizeType x, _SizeType y) const
  {
    if(!_size.IsPointInRect(x, y))
      throw "Field.operator(int, int): range check error!";
    return _values[x][y];
  }

  template <typename _Type, typename _SizeType>
  void Array2d<_Type, _SizeType>::Print(
    const char *separator, const char *caption,
    const char *end_line, const char *end_caption)
  {
    //if (true)
      //std::cout << "(" << _size.width << ", " << _size.height << ") ";
    std::cout << caption;
    for (int i = 0; i < _size.height; ++i) {
      for (int j = 0; j < _size.width; ++j) {
        std::cout << _values[j][i] << separator;
      }
      std::cout << end_line;
    }
    std::cout << end_caption;
  }


}


#endif // VS_ARRAY2D_HPP_INCLUDED
