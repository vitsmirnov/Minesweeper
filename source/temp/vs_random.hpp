/*
  (c) Vitaly Smirnov [VSdev]
  mrmaybelately@gmail.com
  2023
*/

#ifndef VS_RANDOM_HPP_INCLUDED
#define VS_RANDOM_HPP_INCLUDED

#include <cstdlib>


namespace vslib
{


  inline int Random(int min, int max)
  {
    return min + (int)((double)(max - min + 1) * rand() / (RAND_MAX + 1.0));
  }

  inline int Random(int max)
  {
    return Random(0, max);
  }


} // namespace vslib

#endif // VS_RANDOM_HPP_INCLUDED
