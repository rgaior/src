#ifndef __LINEARINTERPOLATOR_H__
#define __LINEARINTERPOLATOR_H__

#include <stdexcept>
#include <vector>

namespace utl {

  template <class T>
  class LinearInterpolator
  {
  public:
    LinearInterpolator(const T _x0, const T _dx, const std::vector<T>& _ys) :
      x0(_x0),
      dx(_dx),
      ys(_ys)
    {
      n = ys.size();
      x1 = x0 + n * dx;
    }


    T Eval(const T x) const
    {
      if (x < x0 || x > x1)
        throw std::runtime_error("Query out of range.");

      const size_t i = (size_t) ((x - x0) / dx);
      const T y1 = ys[i];
      return y1 + (ys[i + 1] - y1) / dx * (x - x0 - i * dx);
    }


    T GetMinX() const
    {
      return x0;
    }


    T GetMaxX() const
    {
      return x1;
    }


  private:
    size_t n;
    T x0, x1, dx;
    std::vector<T> ys;
  };

}

#endif
