#ifndef __INCLUDE_PRIMITIVES_VEC3_HH__
#define __INCLUDE_PRIMITIVES_VEC3_HH__

#include <cmath>
#include <functional>
#include <iostream>
#include <limits>

#include "common.hh"

/**
 * @file vec3.hh
 * Vec3 class implementation
 */

namespace geom
{

/**
 * @class Vec3
 * @brief Vec3 class realization
 *
 * @tparam T - floating point type of coordinates
 */
template <std::floating_point T>
struct Vec3 final
{
  /**
   * @brief Vec3 coordinates
   */
  T x{}, y{}, z{};

  /**
   * @brief Construct a new Vec3 object from 3 coordinates
   *
   * @param[in] coordX x coordinate
   * @param[in] coordY y coordinate
   * @param[in] coordZ z coordinate
   */
  Vec3(T coordX, T coordY, T coordZ) : x(coordX), y(coordY), z(coordZ)
  {}

  /**
   * @brief Construct a new Vec3 object with equals coordinates
   *
   * @param[in] coordX coordinate (default to {})
   */
  explicit Vec3(T coordX = {}) : Vec3(coordX, coordX, coordX)
  {}

  /**
   * @brief Vec3 equality operator
   *
   * @tparam T vector template parameter
   * @param[in] rhs second vector
   * @return true if vectors are equal
   * @return false otherwise
   */
  bool operator==(const Vec3 &rhs) const;

  /**
   * @brief Vec3 inequality operator
   *
   * @tparam T vector template parameter
   * @param[in] rhs second vector
   * @return true if vectors are not equal
   * @return false otherwise
   */
  bool operator!=(const Vec3 &rhs) const;

  /**
   * @brief Overloaded += operator
   * Increments vector coordinates by corresponding coordinates of vec
   * @param[in] vec vector to incremented with
   * @return Vec3& reference to current instance
   */
  Vec3 &operator+=(const Vec3 &vec);

  /**
   * @brief Overloaded -= operator
   * Decrements vector coordinates by corresponding coordinates of vec
   * @param[in] vec vector to decremented with
   * @return Vec3& reference to current instance
   */
  Vec3 &operator-=(const Vec3 &vec);

  /**
   * @brief Unary - operator
   *
   * @return Vec3 negated Vec3 instance
   */
  Vec3 operator-() const;

  /**
   * @brief Overloaded *= by number operator
   *
   * @tparam nType numeric type of value to multiply by
   * @param[in] val value to multiply by
   * @return Vec3& reference to vector instance
   */
  template <Number nType>
  Vec3 &operator*=(nType val);

  /**
   * @brief Overloaded /= by number operator
   *
   * @tparam nType numeric type of value to divide by
   * @param[in] val value to divide by
   * @return Vec3& reference to vector instance
   *
   * @warning Does not check if val equals 0
   */
  template <Number nType>
  Vec3 &operator/=(nType val);

  /**
   * @brief Dot product function
   *
   * @param rhs vector to dot product with
   * @return T dot product of two vectors
   */
  T dot(const Vec3 &rhs) const;

  /**
   * @brief Cross product function
   *
   * @param rhs vector to cross product with
   * @return Vec3 cross product of two vectors
   */
  Vec3 cross(const Vec3 &rhs) const;

  /**
   * @brief Calculate squared length of a vector function
   *
   * @return T length^2
   */
  T length2() const;

  /**
   * @brief Calculate length of a vector function
   *
   * @return T length
   */
  T length() const;

  /**
   * @brief Get normalized vector function
   *
   * @return Vec3 normalized vector
   */
  Vec3 normalized() const;

  /**
   * @brief Normalize vector function
   *
   * @return Vec3& reference to instance
   */
  Vec3 &normalize() &;

  /**
   * @brief Overloaded operator [] (non-const version)
   * To get access to coordinates
   * @param i index of coordinate (0 - x, 1 - y, 2 - z)
   * @return T& reference to coordinate value
   *
   * @note Coordinates calculated by mod 3
   */
  T &operator[](std::size_t i) &;

  /**
   * @brief Overloaded operator [] (const version)
   * To get access to coordinates
   * @param i index of coordinate (0 - x, 1 - y, 2 - z)
   * @return T coordinate value
   *
   * @note Coordinates calculated by mod 3
   */
  T operator[](std::size_t i) const &;

  /**
   * @brief Overloaded operator [] (rvalue `this` version)
   * To get access to coordinates
   * @param i index of coordinate (0 - x, 1 - y, 2 - z)
   * @return T coordinate value
   *
   * @note Coordinates calculated by mod 3
   */
  T &&operator[](std::size_t i) &&;

  /**
   * @brief Check if vector is parallel to another
   *
   * @param[in] rhs vector to check parallelism with
   * @return true if vector is parallel
   * @return false otherwise
   */
  bool isPar(const Vec3 &rhs) const;

  /**
   * @brief Check if vector is perpendicular to another
   *
   * @param[in] rhs vector to check perpendicularity with
   * @return true if vector is perpendicular
   * @return false otherwise
   */
  bool isPerp(const Vec3 &rhs) const;

  /**
   * @brief Check if vector is equal to another
   *
   * @param[in] rhs vector to check equality with
   * @return true if vector is equal
   * @return false otherwise
   */
  bool isEqual(const Vec3 &rhs) const;
};

/**
 * @brief Overloaded + operator
 *
 * @tparam T vector template parameter
 * @param[in] lhs first vector
 * @param[in] rhs second vector
 * @return Vec3<T> sum of two vectors
 */
template <std::floating_point T>
Vec3<T> operator+(const Vec3<T> &lhs, const Vec3<T> &rhs)
{
  Vec3<T> res{lhs};
  res += rhs;
  return res;
}

/**
 * @brief Overloaded - operator
 *
 * @tparam T vector template parameter
 * @param[in] lhs first vector
 * @param[in] rhs second vector
 * @return Vec3<T> res of two vectors
 */
template <std::floating_point T>
Vec3<T> operator-(const Vec3<T> &lhs, const Vec3<T> &rhs)
{
  Vec3<T> res{lhs};
  res -= rhs;
  return res;
}

/**
 * @brief Overloaded multiple by value operator
 *
 * @tparam nT type of value to multiply by
 * @tparam T vector template parameter
 * @param[in] val value to multiply by
 * @param[in] rhs vector to multiply by value
 * @return Vec3<T> result vector
 */
template <Number nT, std::floating_point T>
Vec3<T> operator*(const nT &val, const Vec3<T> &rhs)
{
  Vec3<T> res{rhs};
  res *= val;
  return res;
}

/**
 * @brief Overloaded multiple by value operator
 *
 * @tparam nT type of value to multiply by
 * @tparam T vector template parameter
 * @param[in] val value to multiply by
 * @param[in] lhs vector to multiply by value
 * @return Vec3<T> result vector
 */
template <Number nT, std::floating_point T>
Vec3<T> operator*(const Vec3<T> &lhs, const nT &val)
{
  Vec3<T> res{lhs};
  res *= val;
  return res;
}

/**
 * @brief Overloaded divide by value operator
 *
 * @tparam nT type of value to divide by
 * @tparam T vector template parameter
 * @param[in] val value to divide by
 * @param[in] lhs vector to divide by value
 * @return Vec3<T> result vector
 */
template <Number nT, std::floating_point T>
Vec3<T> operator/(const Vec3<T> &lhs, const nT &val)
{
  Vec3<T> res{lhs};
  res /= val;
  return res;
}

/**
 * @brief Dot product function
 *
 * @tparam T vector template parameter
 * @param[in] lhs first vector
 * @param[in] rhs second vector
 * @return T dot production
 */
template <std::floating_point T>
T dot(const Vec3<T> &lhs, const Vec3<T> &rhs)
{
  return lhs.dot(rhs);
}

/**
 * @brief Cross product function
 *
 * @tparam T vector template parameter
 * @param[in] lhs first vector
 * @param[in] rhs second vector
 * @return T cross production
 */
template <std::floating_point T>
Vec3<T> cross(const Vec3<T> &lhs, const Vec3<T> &rhs)
{
  return lhs.cross(rhs);
}

/**
 * @brief Triple product function
 *
 * @tparam T vector template parameter
 * @param[in] v1 first vector
 * @param[in] v2 second vector
 * @param[in] v3 third vector
 * @return T triple production
 */
template <std::floating_point T>
T triple(const Vec3<T> &v1, const Vec3<T> &v2, const Vec3<T> &v3)
{
  return dot(v1, cross(v2, v3));
}

/**
 * @brief Vec3 print operator
 *
 * @tparam T vector template parameter
 * @param[in, out] ost output stream
 * @param[in] vec vector to print
 * @return std::ostream& modified stream instance
 */
template <std::floating_point T>
std::ostream &operator<<(std::ostream &ost, const Vec3<T> &vec)
{
  ost << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
  return ost;
}

/**
 * @brief Vec3 scan operator
 *
 * @tparam T vector template parameter
 * @param[in, out] ist input stram
 * @param[in, out] vec vector to scan
 * @return std::istream& modified stream instance
 */
template <std::floating_point T>
std::istream &operator>>(std::istream &ist, Vec3<T> &vec)
{
  ist >> vec.x >> vec.y >> vec.z;
  return ist;
}

using Vec3D = Vec3<double>;
using Vec3F = Vec3<float>;

template <std::floating_point T>
bool Vec3<T>::operator==(const Vec3 &rhs) const
{
  return isEqual(rhs);
}

template <std::floating_point T>
bool Vec3<T>::operator!=(const Vec3 &rhs) const
{
  return !operator==(rhs);
}

template <std::floating_point T>
Vec3<T> &Vec3<T>::operator+=(const Vec3 &vec)
{
  x += vec.x;
  y += vec.y;
  z += vec.z;

  return *this;
}

template <std::floating_point T>
Vec3<T> &Vec3<T>::operator-=(const Vec3 &vec)
{
  x -= vec.x;
  y -= vec.y;
  z -= vec.z;

  return *this;
}

template <std::floating_point T>
Vec3<T> Vec3<T>::operator-() const
{
  return Vec3{-x, -y, -z};
}

template <std::floating_point T>
template <Number nType>
Vec3<T> &Vec3<T>::operator*=(nType val)
{
  auto fval = static_cast<T>(val);
  x *= fval;
  y *= fval;
  z *= fval;

  return *this;
}

template <std::floating_point T>
template <Number nType>
Vec3<T> &Vec3<T>::operator/=(nType val)
{
  auto fval = static_cast<T>(val);
  x /= fval;
  y /= fval;
  z /= fval;

  return *this;
}

template <std::floating_point T>
T Vec3<T>::dot(const Vec3 &rhs) const
{
  return x * rhs.x + y * rhs.y + z * rhs.z;
}

template <std::floating_point T>
Vec3<T> Vec3<T>::cross(const Vec3 &rhs) const
{
  return Vec3{y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x};
}

template <std::floating_point T>
T Vec3<T>::length2() const
{
  return dot(*this);
}

template <std::floating_point T>
T Vec3<T>::length() const
{
  return std::sqrt(length2());
}

template <std::floating_point T>
Vec3<T> Vec3<T>::normalized() const
{
  Vec3 res{*this};
  res.normalize();
  return res;
}

template <std::floating_point T>
Vec3<T> &Vec3<T>::normalize() &
{
  T len2 = length2();
  if (isZeroThreshold(len2) || isEqualThreshold(len2, T{1}))
    return *this;
  return *this /= std::sqrt(len2);
}

template <std::floating_point T>
T &Vec3<T>::operator[](std::size_t i) &
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
T Vec3<T>::operator[](std::size_t i) const &
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
T &&Vec3<T>::operator[](std::size_t i) &&
{
  switch (i % 3)
  {
  case 0:
    return std::move(x);
  case 1:
    return std::move(y);
  case 2:
    return std::move(z);
  default:
    throw std::logic_error{"Impossible case in operator[]\n"};
  }
}

template <std::floating_point T>
bool Vec3<T>::isPar(const Vec3 &rhs) const
{
  return cross(rhs).isEqual(Vec3<T>{0});
}

template <std::floating_point T>
bool Vec3<T>::isPerp(const Vec3 &rhs) const
{
  return isZeroThreshold(dot(rhs));
}

template <std::floating_point T>
bool Vec3<T>::isEqual(const Vec3 &rhs) const
{
  return isEqualThreshold(x, rhs.x) && isEqualThreshold(y, rhs.y) && isEqualThreshold(z, rhs.z);
}

} // namespace geom

#endif // __INCLUDE_PRIMITIVES_VEC3_HH__
