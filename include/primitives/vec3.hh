#ifndef __INCLUDE_PRIMITIVES_VEC3_HH__
#define __INCLUDE_PRIMITIVES_VEC3_HH__

#include <cmath>
#include <concepts>
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
private:
  /**
   * @brief Threshold static variable for numbers comparision
   */
  static inline T threshold_ = 1e3 * std::numeric_limits<T>::epsilon();

public:
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
  Vec3 &normalize();

  /**
   * @brief Overloaded operator [] (non-const version)
   * To get access to coordinates
   * @param i index of coordinate (0 - x, 1 - y, 2 - z)
   * @return T& reference to coordinate value
   *
   * @note Coordinates calculated by mod 3
   */
  T &operator[](size_t i);

  /**
   * @brief Overloaded operator [] (const version)
   * To get access to coordinates
   * @param i index of coordinate (0 - x, 1 - y, 2 - z)
   * @return T coordinate value
   *
   * @note Coordinates calculated by mod 3
   */
  T operator[](size_t i) const;

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
   *
   * @note Equality check performs using isNumEq(T lhs, T rhs) function
   */
  bool isEqual(const Vec3 &rhs) const;

  /**
   * @brief Check equality (with threshold) of two floating point numbers function
   *
   * @param[in] lhs first number
   * @param[in] rhs second number
   * @return true if numbers equals with threshold (|lhs - rhs| < threshold)
   * @return false otherwise
   *
   * @note Threshold defined by threshold_ static member
   */
  static bool isNumEq(T lhs, T rhs);

  /**
   * @brief Set new threshold value
   *
   * @param[in] thres value to set
   */
  static void setThreshold(T thres);

  /**
   * @brief Get current threshold value
   */
  static void getThreshold();

  /**
   * @brief Set threshold to default value
   * @note default value equals float point epsilon
   */
  static void setDefThreshold();
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
 * @brief Vec3 equality operator
 *
 * @tparam T vector template parameter
 * @param[in] lhs first vector
 * @param[in] rhs second vector
 * @return true if vectors are equal
 * @return false otherwise
 */
template <std::floating_point T>
bool operator==(const Vec3<T> &lhs, const Vec3<T> &rhs)
{
  return lhs.isEqual(rhs);
}

/**
 * @brief Vec3 inequality operator
 *
 * @tparam T vector template parameter
 * @param[in] lhs first vector
 * @param[in] rhs second vector
 * @return true if vectors are not equal
 * @return false otherwise
 */
template <std::floating_point T>
bool operator!=(const Vec3<T> &lhs, const Vec3<T> &rhs)
{
  return !(lhs == rhs);
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

using Vec3D = Vec3<double>;
using Vec3F = Vec3<float>;

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
  x *= val;
  y *= val;
  z *= val;

  return *this;
}

template <std::floating_point T>
template <Number nType>
Vec3<T> &Vec3<T>::operator/=(nType val)
{
  x /= static_cast<T>(val);
  y /= static_cast<T>(val);
  z /= static_cast<T>(val);

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
Vec3<T> &Vec3<T>::normalize()
{
  T len2 = length2();
  if (isNumEq(len2, 0) || isNumEq(len2, 1))
    return *this;
  return *this /= std::sqrt(len2);
}

template <std::floating_point T>
T &Vec3<T>::operator[](size_t i)
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
T Vec3<T>::operator[](size_t i) const
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
bool Vec3<T>::isPar(const Vec3 &rhs) const
{
  return cross(rhs).isEqual(Vec3<T>{0});
}

template <std::floating_point T>
bool Vec3<T>::isPerp(const Vec3 &rhs) const
{
  return isNumEq(dot(rhs), 0);
}

template <std::floating_point T>
bool Vec3<T>::isEqual(const Vec3 &rhs) const
{
  return isNumEq(x, rhs.x) && isNumEq(y, rhs.y) && isNumEq(z, rhs.z);
}

template <std::floating_point T>
bool Vec3<T>::isNumEq(T lhs, T rhs)
{
  return std::abs(rhs - lhs) < threshold_;
}

template <std::floating_point T>
void Vec3<T>::setThreshold(T thres)
{
  threshold_ = thres;
}

template <std::floating_point T>
void Vec3<T>::getThreshold()
{
  return threshold_;
}

template <std::floating_point T>
void Vec3<T>::setDefThreshold()
{
  threshold_ = std::numeric_limits<T>::epsilon();
}

} // namespace geom

#endif // __INCLUDE_PRIMITIVES_VEC3_HH__
