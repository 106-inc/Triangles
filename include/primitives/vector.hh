#ifndef __INCLUDE_PRIMITIVES_VECTOR_HH__
#define __INCLUDE_PRIMITIVES_VECTOR_HH__

#include <cmath>
#include <concepts>
#include <functional>
#include <iostream>
#include <limits>

namespace geom
{

template <class T>
concept Number = std::is_floating_point_v<T> || std::is_integral_v<T>;

template <std::floating_point T>
struct Vector final
{
private:
  static inline T threshold_ = 1e3 * std::numeric_limits<T>::epsilon();

public:
  T x{}, y{}, z{};

  Vector(T coordX, T coordY, T coordZ) : x(coordX), y(coordY), z(coordZ)
  {
  }

  explicit Vector(T coordX = {}) : Vector(coordX, coordX, coordX)
  {
  }

  Vector &operator+=(const Vector &vec);
  Vector &operator-=(const Vector &vec);
  Vector operator-() const;

  template <Number nType>
  Vector &operator*=(nType val);

  template <Number nType>
  Vector &operator/=(nType val);

  T dot(const Vector &rhs) const;
  Vector cross(const Vector &rhs) const;

  T length2() const;
  T length() const;

  Vector normalized() const;
  Vector &normalize();

  T &operator[](size_t i);
  T operator[](size_t i) const;

  /* TODO: think of adding isPerpendicular and getAngle(const Vector &rhs) */
  bool isParallel(const Vector &rhs) const;
  bool isEqual(const Vector &rhs) const;
  static bool isNumEq(T lhs, T rhs);

  static void setThreshold(T thres);
  static void getThreshold();
  static void setDefThreshold();
};

template <std::floating_point T>
Vector<T> &Vector<T>::operator+=(const Vector &vec)
{
  x += vec.x;
  y += vec.y;
  z += vec.z;

  return *this;
}

template <std::floating_point T>
Vector<T> &Vector<T>::operator-=(const Vector &vec)
{
  x -= vec.x;
  y -= vec.y;
  z -= vec.z;

  return *this;
}

template <std::floating_point T>
Vector<T> Vector<T>::operator-() const
{
  return Vector{-x, -y, -z};
}

template <std::floating_point T>
template <Number nType>
Vector<T> &Vector<T>::operator*=(nType val)
{
  x *= val;
  y *= val;
  z *= val;

  return *this;
}

template <std::floating_point T>
template <Number nType>
Vector<T> &Vector<T>::operator/=(nType val)
{
  x /= val;
  y /= val;
  z /= val;

  return *this;
}

template <std::floating_point T>
T Vector<T>::dot(const Vector &rhs) const
{
  return x * rhs.x + y * rhs.y + z * rhs.z;
}

template <std::floating_point T>
Vector<T> Vector<T>::cross(const Vector &rhs) const
{
  return Vector{y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x};
}

template <std::floating_point T>
T Vector<T>::length2() const
{
  return dot(*this);
}

template <std::floating_point T>
T Vector<T>::length() const
{
  return std::sqrt(length2());
}

template <std::floating_point T>
Vector<T> Vector<T>::normalized() const
{
  Vector res{*this};
  res.normalize();
  return res;
}

template <std::floating_point T>
Vector<T> &Vector<T>::normalize()
{
  T len2 = length2();
  if (isNumEq(len2, 0) || isNumEq(len2, 1))
    return *this;
  return *this /= std::sqrt(len2);
}

template <std::floating_point T>
T &Vector<T>::operator[](size_t i)
{
  switch (i % 3)
  {
  case 0:
    return x;
  case 1:
    return y;
  case 2:
    return z;
  default:
    throw std::logic_error{"Impossible case in operator[]\n"};
  }
}

template <std::floating_point T>
T Vector<T>::operator[](size_t i) const
{
  switch (i % 3)
  {
  case 0:
    return x;
  case 1:
    return y;
  case 2:
    return z;
  default:
    throw std::logic_error{"Impossible case in operator[]\n"};
  }
}

template <std::floating_point T>
bool Vector<T>::isParallel(const Vector &rhs) const
{
  return cross(rhs).isEqual(Vector<T>{0});
}

template <std::floating_point T>
bool Vector<T>::isEqual(const Vector &rhs) const
{
  return isNumEq(x, rhs.x) && isNumEq(y, rhs.y) && isNumEq(z, rhs.z);
}

template <std::floating_point T>
bool Vector<T>::isNumEq(T lhs, T rhs)
{
  return std::abs(rhs - lhs) < threshold_;
}

template <std::floating_point T>
void Vector<T>::setThreshold(T thres)
{
  threshold_ = thres;
}

template <std::floating_point T>
void Vector<T>::getThreshold()
{
  return threshold_;
}

template <std::floating_point T>
void Vector<T>::setDefThreshold()
{
  threshold_ = std::numeric_limits<T>::epsilon();
}

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

template <Number nT, std::floating_point T>
Vector<T> operator*(const nT &val, const Vector<T> &rhs)
{
  Vector<T> res{rhs};
  res *= val;
  return res;
}

template <Number nT, std::floating_point T>
Vector<T> operator*(const Vector<T> &lhs, const nT &val)
{
  Vector<T> res{lhs};
  res *= val;
  return res;
}

template <Number nT, std::floating_point T>
Vector<T> operator/(const Vector<T> &lhs, const nT &val)
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
bool operator==(const Vector<T> &lhs, const Vector<T> &rhs)
{
  return lhs.isEqual(rhs);
}

template <std::floating_point T>
bool operator!=(const Vector<T> &lhs, const Vector<T> &rhs)
{
  return !(lhs == rhs);
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

#endif // __INCLUDE_PRIMITIVES_VECTOR_HH__
