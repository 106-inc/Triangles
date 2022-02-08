#ifndef __INCLUDE_GEOM_VECTOR_HH__
#define __INCLUDE_GEOM_VECTOR_HH__

#include <cmath>
#include <concepts>
#include <functional>
#include <iostream>
#include <limits>

namespace geom
{
template <std::floating_point T>
bool isEqual(T lhs, T rhs, T eps)
{
  return std::abs(rhs - lhs) < eps;
}

template <std::floating_point T>
struct Vector final
{
private:
  static inline T threshold_ = std::numeric_limits<T>::epsilon();

public:
  T x{}, y{}, z{};

  Vector(T x, T y, T z) : x(x), y(y), z(z)
  {
  }

  explicit Vector(T x = {}) : Vector(x, x, x)
  {
  }

  Vector &operator+=(const Vector &vec)
  {
    x += vec.x;
    y += vec.y;
    z += vec.z;

    return *this;
  }

  Vector &operator-=(const Vector &vec)
  {
    x -= vec.x;
    y -= vec.y;
    z -= vec.z;

    return *this;
  }

  Vector operator-() const
  {
    return Vector{-x, -y, -z};
  }

  template <std::integral iType>
  Vector &operator*=(iType val)
  {
    x *= val;
    y *= val;
    z *= val;

    return *this;
  }

  template <std::integral iType>
  Vector &operator/=(iType val)
  {
    x /= val;
    y /= val;
    z /= val;

    return *this;
  }

  T dot(const Vector &rhs) const
  {
    return x * rhs.x + y * rhs.y + z * rhs.z;
  }

  Vector cross(const Vector &rhs) const
  {
    return Vector{y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x};
  }

  T length2() const
  {
    return dot(*this);
  }

  T length() const
  {
    return std::sqrt(length2());
  }

  Vector normalizing() const
  {
    Vector res{*this};
    res.normalize();
    return res;
  }

  Vector &normalize()
  {
    T len = length();
    if (isEq(len, 0) || isEq(len, 1))
      return *this;
    return *this /= len;
  }

  T &operator[](size_t i)
  {
    switch (i % 3)
    {
    case 0:
      return x;
    case 1:
      return y;
    case 2:
      return z;
    }

    // Dummy return
    return x;
  }

  T operator[](size_t i) const
  {
    switch (i % 3)
    {
    case 0:
      return x;
    case 1:
      return y;
    case 2:
      return z;
    }

    // Dummy return
    return x;
  }

  bool isEq(const Vector &rhs)
  {
    auto res = true;
    for (size_t i = 0; i < 3; ++i)
      res = res && isEqual(operator[](i), rhs[i], threshold_);
    return res;
  }
};

template <std::floating_point T>
Vector<T> operator+(const Vector<T> &lhs, const Vector<T> &rhs)
{
  Vector<T> res{lhs};
  res += rhs;
  return res;
}

template <std::floating_point T>
Vector<T> operator-(const Vector<T> &lhs, const Vector<T> &rhs)
{
  Vector<T> res{lhs};
  res -= rhs;
  return res;
}

template <std::integral iT, std::floating_point T>
Vector<T> operator*(const iT &val, const Vector<T> &rhs)
{
  Vector<T> res{rhs};
  res *= val;
  return res;
}

template <std::integral iT, std::floating_point T>
Vector<T> operator*(const Vector<T> &lhs, const iT &val)
{
  Vector<T> res{lhs};
  res *= val;
  return res;
}

template <std::integral iT, std::floating_point T>
Vector<T> operator/(const Vector<T> &lhs, const iT &val)
{
  Vector<T> res{lhs};
  res /= val;
  return res;
}

template <std::floating_point T>
T operator&(const Vector<T> &lhs, const Vector<T> &rhs)
{
  return lhs.dot(rhs);
}

template <std::floating_point T>
Vector<T> operator%(const Vector<T> &lhs, const Vector<T> &rhs)
{
  return lhs.cross(rhs);
}

template <std::floating_point T>
std::ostream &operator<<(std::ostream &ost, const Vector<T> &vec)
{
  ost << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")" << std::endl;
  return ost;
}

using VectorD = Vector<double>;
using VectorF = Vector<float>;

} // namespace geom

#endif // __INCLUDE_GEOM_VECTOR_HH__