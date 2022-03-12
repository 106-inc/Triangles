#ifndef __INCLUDE_PRIMITIVES_VEC2_HH__
#define __INCLUDE_PRIMITIVES_VEC2_HH__

#include <cmath>
#include <concepts>
#include <functional>
#include <iostream>
#include <limits>

#include "common.hh"

/**
 * @file vec2.hh
 * Vec2 class implementation
 */

namespace geom
{

/**
 * @class Vec2
 * @brief Vec2 class realization
 *
 * @tparam T - floating point type of coordinates
 */
template <std::floating_point T>
struct Vec2 final
{
private:
  /**
   * @brief Threshold static variable for numbers comparision
   */
  static inline T threshold_ = 1e3 * std::numeric_limits<T>::epsilon();

public:
  /**
   * @brief Vec2 coordinates
   */
  T x{}, y{};

  /**
   * @brief Construct a new Vec2 object from 3 coordinates
   *
   * @param[in] coordX x coordinate
   * @param[in] coordY y coordinate
   */
  Vec2(T coordX, T coordY) : x(coordX), y(coordY)
  {}

  /**
   * @brief Construct a new Vec2 object with equals coordinates
   *
   * @param[in] coordX coordinate (default to {})
   */
  explicit Vec2(T coordX = {}) : Vec2(coordX, coordX)
  {}

  /**
   * @brief Overloaded += operator
   * Increments vector coordinates by corresponding coordinates of vec
   * @param[in] vec vector to incremented with
   * @return Vec2& reference to current instance
   */
  Vec2 &operator+=(const Vec2 &vec);

  /**
   * @brief Overloaded -= operator
   * Decrements vector coordinates by corresponding coordinates of vec
   * @param[in] vec vector to decremented with
   * @return Vec2& reference to current instance
   */
  Vec2 &operator-=(const Vec2 &vec);

  /**
   * @brief Unary - operator
   *
   * @return Vec2 negated Vec2 instance
   */
  Vec2 operator-() const;

  /**
   * @brief Overloaded *= by number operator
   *
   * @tparam nType numeric type of value to multiply by
   * @param[in] val value to multiply by
   * @return Vec2& reference to vector instance
   */
  template <Number nType>
  Vec2 &operator*=(nType val);

  /**
   * @brief Overloaded /= by number operator
   *
   * @tparam nType numeric type of value to divide by
   * @param[in] val value to divide by
   * @return Vec2& reference to vector instance
   *
   * @warning Does not check if val equals 0
   */
  template <Number nType>
  Vec2 &operator/=(nType val);

  /**
   * @brief Dot product function
   *
   * @param rhs vector to dot product with
   * @return T dot product of two vectors
   */
  T dot(const Vec2 &rhs) const;

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
   * @brief Get the perpendicular to this vector
   *
   * @return Vec2 perpendicular vector
   */
  Vec2 getPerp() const;

  /**
   * @brief Get normalized vector function
   *
   * @return Vec2 normalized vector
   */
  Vec2 normalized() const;

  /**
   * @brief Normalize vector function
   *
   * @return Vec2& reference to instance
   */
  Vec2 &normalize();

  /**
   * @brief Overloaded operator [] (non-const version)
   * To get access to coordinates
   * @param i index of coordinate (0 - x, 1 - y)
   * @return T& reference to coordinate value
   *
   * @note Coordinates calculated by mod 2
   */
  T &operator[](size_t i);

  /**
   * @brief Overloaded operator [] (const version)
   * To get access to coordinates
   * @param i index of coordinate (0 - x, 1 - y)
   * @return T coordinate value
   *
   * @note Coordinates calculated by mod 2
   */
  T operator[](size_t i) const;

  /**
   * @brief Check if vector is parallel to another
   *
   * @param[in] rhs vector to check parallelism with
   * @return true if vector is parallel
   * @return false otherwise
   */
  bool isPar(const Vec2 &rhs) const;

  /**
   * @brief Check if vector is perpendicular to another
   *
   * @param[in] rhs vector to check perpendicularity with
   * @return true if vector is perpendicular
   * @return false otherwise
   */
  bool isPerp(const Vec2 &rhs) const;

  /**
   * @brief Check if vector is equal to another
   *
   * @param[in] rhs vector to check equality with
   * @return true if vector is equal
   * @return false otherwise
   *
   * @note Equality check performs using isNumEq(T lhs, T rhs) function
   */
  bool isEqual(const Vec2 &rhs) const;

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
 * @return Vec2<T> sum of two vectors
 */
template <std::floating_point T>
Vec2<T> operator+(const Vec2<T> &lhs, const Vec2<T> &rhs)
{
  Vec2<T> res{lhs};
  res += rhs;
  return res;
}

/**
 * @brief Overloaded - operator
 *
 * @tparam T vector template parameter
 * @param[in] lhs first vector
 * @param[in] rhs second vector
 * @return Vec2<T> res of two vectors
 */
template <std::floating_point T>
Vec2<T> operator-(const Vec2<T> &lhs, const Vec2<T> &rhs)
{
  Vec2<T> res{lhs};
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
 * @return Vec2<T> result vector
 */
template <Number nT, std::floating_point T>
Vec2<T> operator*(const nT &val, const Vec2<T> &rhs)
{
  Vec2<T> res{rhs};
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
 * @return Vec2<T> result vector
 */
template <Number nT, std::floating_point T>
Vec2<T> operator*(const Vec2<T> &lhs, const nT &val)
{
  Vec2<T> res{lhs};
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
 * @return Vec2<T> result vector
 */
template <Number nT, std::floating_point T>
Vec2<T> operator/(const Vec2<T> &lhs, const nT &val)
{
  Vec2<T> res{lhs};
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
T dot(const Vec2<T> &lhs, const Vec2<T> &rhs)
{
  return lhs.dot(rhs);
}

/**
 * @brief Vec2 equality operator
 *
 * @tparam T vector template parameter
 * @param[in] lhs first vector
 * @param[in] rhs second vector
 * @return true if vectors are equal
 * @return false otherwise
 */
template <std::floating_point T>
bool operator==(const Vec2<T> &lhs, const Vec2<T> &rhs)
{
  return lhs.isEqual(rhs);
}

/**
 * @brief Vec2 inequality operator
 *
 * @tparam T vector template parameter
 * @param[in] lhs first vector
 * @param[in] rhs second vector
 * @return true if vectors are not equal
 * @return false otherwise
 */
template <std::floating_point T>
bool operator!=(const Vec2<T> &lhs, const Vec2<T> &rhs)
{
  return !(lhs == rhs);
}

/**
 * @brief Vec2 print operator
 *
 * @tparam T vector template parameter
 * @param[in, out] ost output stream
 * @param[in] vec vector to print
 * @return std::ostream& modified stream instance
 */
template <std::floating_point T>
std::ostream &operator<<(std::ostream &ost, const Vec2<T> &vec)
{
  ost << "(" << vec.x << ", " << vec.y << ")";
  return ost;
}

using Vec2D = Vec2<double>;
using Vec2F = Vec2<float>;

template <std::floating_point T>
Vec2<T> &Vec2<T>::operator+=(const Vec2 &vec)
{
  x += vec.x;
  y += vec.y;

  return *this;
}

template <std::floating_point T>
Vec2<T> &Vec2<T>::operator-=(const Vec2 &vec)
{
  x -= vec.x;
  y -= vec.y;

  return *this;
}

template <std::floating_point T>
Vec2<T> Vec2<T>::operator-() const
{
  return Vec2{-x, -y};
}

template <std::floating_point T>
template <Number nType>
Vec2<T> &Vec2<T>::operator*=(nType val)
{
  x *= val;
  y *= val;

  return *this;
}

template <std::floating_point T>
template <Number nType>
Vec2<T> &Vec2<T>::operator/=(nType val)
{
  x /= static_cast<T>(val);
  y /= static_cast<T>(val);

  return *this;
}

template <std::floating_point T>
T Vec2<T>::dot(const Vec2 &rhs) const
{
  return x * rhs.x + y * rhs.y;
}

template <std::floating_point T>
T Vec2<T>::length2() const
{
  return dot(*this);
}

template <std::floating_point T>
T Vec2<T>::length() const
{
  return std::sqrt(length2());
}

template <std::floating_point T>
Vec2<T> Vec2<T>::getPerp() const
{
  return {y, -x};
}

template <std::floating_point T>
Vec2<T> Vec2<T>::normalized() const
{
  Vec2 res{*this};
  res.normalize();
  return res;
}

template <std::floating_point T>
Vec2<T> &Vec2<T>::normalize()
{
  T len2 = length2();
  if (isNumEq(len2, 0) || isNumEq(len2, 1))
    return *this;
  return *this /= std::sqrt(len2);
}

template <std::floating_point T>
T &Vec2<T>::operator[](size_t i)
{
  switch (i % 3)
  {
  case 0:
    return x;
  case 1:
    return y;
  default:
    throw std::logic_error{"Impossible case in operator[]\n"};
  }
}

template <std::floating_point T>
T Vec2<T>::operator[](size_t i) const
{
  switch (i % 3)
  {
  case 0:
    return x;
  case 1:
    return y;
  default:
    throw std::logic_error{"Impossible case in operator[]\n"};
  }
}

template <std::floating_point T>
bool Vec2<T>::isPar(const Vec2 &rhs) const
{
  auto det = x * rhs.y - rhs.x * y;
  return isNumEq(det, 0);
}

template <std::floating_point T>
bool Vec2<T>::isPerp(const Vec2 &rhs) const
{
  return isNumEq(dot(rhs), 0);
}

template <std::floating_point T>
bool Vec2<T>::isEqual(const Vec2 &rhs) const
{
  return isNumEq(x, rhs.x) && isNumEq(y, rhs.y);
}

template <std::floating_point T>
bool Vec2<T>::isNumEq(T lhs, T rhs)
{
  return std::abs(rhs - lhs) < threshold_;
}

template <std::floating_point T>
void Vec2<T>::setThreshold(T thres)
{
  threshold_ = thres;
}

template <std::floating_point T>
void Vec2<T>::getThreshold()
{
  return threshold_;
}

template <std::floating_point T>
void Vec2<T>::setDefThreshold()
{
  threshold_ = std::numeric_limits<T>::epsilon();
}

} // namespace geom

#endif // __INCLUDE_PRIMITIVES_VEC2_HH__
