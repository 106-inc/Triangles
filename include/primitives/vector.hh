#ifndef __INCLUDE_PRIMITIVES_VECTOR_HH__
#define __INCLUDE_PRIMITIVES_VECTOR_HH__

#include <cmath>
#include <concepts>
#include <functional>
#include <iostream>
#include <limits>

/**
 * @file vector.hh
 * Vector class implementation
 */

namespace geom
{

/**
 * @concept Number
 * @brief Useful concept which represents floating point and integral types
 *
 * @tparam T
 */
template <class T>
concept Number = std::is_floating_point_v<T> || std::is_integral_v<T>;

/**
 * @class Vector
 * @brief Vector class realization
 *
 * @tparam T - floating point type of coordinates
 */
template <std::floating_point T>
struct Vector final
{
private:
  /**
   * @brief Threshold static variable for numbers comparision
   */
  static inline T threshold_ = 1e3 * std::numeric_limits<T>::epsilon();

public:
  /**
   * @brief Vector coordinates
   */
  T x{}, y{}, z{};

  /**
   * @brief Construct a new Vector object from 3 coordinates
   *
   * @param[in] coordX x coordinate
   * @param[in] coordY y coordinate
   * @param[in] coordZ z coordinate
   */
  Vector(T coordX, T coordY, T coordZ) : x(coordX), y(coordY), z(coordZ)
  {}

  /**
   * @brief Construct a new Vector object with equals coordinates
   *
   * @param[in] coordX coordinate (default to {})
   */
  explicit Vector(T coordX = {}) : Vector(coordX, coordX, coordX)
  {}

  /**
   * @brief Overloaded += operator
   * Increments vector coordinates by corresponding coordinates of vec
   * @param[in] vec vector to incremented with
   * @return Vector& reference to current instance
   */
  Vector &operator+=(const Vector &vec);

  /**
   * @brief Overloaded -= operator
   * Decrements vector coordinates by corresponding coordinates of vec
   * @param[in] vec vector to decremented with
   * @return Vector& reference to current instance
   */
  Vector &operator-=(const Vector &vec);

  /**
   * @brief Unary - operator
   *
   * @return Vector negated Vector instance
   */
  Vector operator-() const;

  /**
   * @brief Overloaded *= by number operator
   *
   * @tparam nType numeric type of value to multiply by
   * @param[in] val value to multiply by
   * @return Vector& reference to vector instance
   */
  template <Number nType>
  Vector &operator*=(nType val);

  /**
   * @brief Overloaded /= by number operator
   *
   * @tparam nType numeric type of value to divide by
   * @param[in] val value to divide by
   * @return Vector& reference to vector instance
   *
   * @warning Does not check if val equals 0
   */
  template <Number nType>
  Vector &operator/=(nType val);

  /**
   * @brief Dot product function
   *
   * @param rhs vector to dot product with
   * @return T dot product of two vectors
   */
  T dot(const Vector &rhs) const;

  /**
   * @brief Cross product function
   *
   * @param rhs vector to cross product with
   * @return Vector cross product of two vectors
   */
  Vector cross(const Vector &rhs) const;

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
   * @return Vector normalized vector
   */
  Vector normalized() const;

  /**
   * @brief Normalize vector function
   *
   * @return Vector& reference to instance
   */
  Vector &normalize();

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
  bool isPar(const Vector &rhs) const;

  /**
   * @brief Check if vector is perpendicular to another
   *
   * @param[in] rhs vector to check perpendicularity with
   * @return true if vector is perpendicular
   * @return false otherwise
   */
  bool isPerp(const Vector &rhs) const;

  /**
   * @brief Check if vector is equal to another
   *
   * @param[in] rhs vector to check equality with
   * @return true if vector is equal
   * @return false otherwise
   *
   * @note Equality check performs using isNumEq(T lhs, T rhs) function
   */
  bool isEqual(const Vector &rhs) const;

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
 * @return Vector<T> sum of two vectors
 */
template <std::floating_point T>
Vector<T> operator+(const Vector<T> &lhs, const Vector<T> &rhs)
{
  Vector<T> res{lhs};
  res += rhs;
  return res;
}

/**
 * @brief Overloaded - operator
 *
 * @tparam T vector template parameter
 * @param[in] lhs first vector
 * @param[in] rhs second vector
 * @return Vector<T> res of two vectors
 */
template <std::floating_point T>
Vector<T> operator-(const Vector<T> &lhs, const Vector<T> &rhs)
{
  Vector<T> res{lhs};
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
 * @return Vector<T> result vector
 */
template <Number nT, std::floating_point T>
Vector<T> operator*(const nT &val, const Vector<T> &rhs)
{
  Vector<T> res{rhs};
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
 * @return Vector<T> result vector
 */
template <Number nT, std::floating_point T>
Vector<T> operator*(const Vector<T> &lhs, const nT &val)
{
  Vector<T> res{lhs};
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
 * @return Vector<T> result vector
 */
template <Number nT, std::floating_point T>
Vector<T> operator/(const Vector<T> &lhs, const nT &val)
{
  Vector<T> res{lhs};
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
T dot(const Vector<T> &lhs, const Vector<T> &rhs)
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
Vector<T> cross(const Vector<T> &lhs, const Vector<T> &rhs)
{
  return lhs.cross(rhs);
}

/**
 * @brief Vector equality operator
 *
 * @tparam T vector template parameter
 * @param[in] lhs first vector
 * @param[in] rhs second vector
 * @return true if vectors are equal
 * @return false otherwise
 */
template <std::floating_point T>
bool operator==(const Vector<T> &lhs, const Vector<T> &rhs)
{
  return lhs.isEqual(rhs);
}

/**
 * @brief Vector inequality operator
 *
 * @tparam T vector template parameter
 * @param[in] lhs first vector
 * @param[in] rhs second vector
 * @return true if vectors are not equal
 * @return false otherwise
 */
template <std::floating_point T>
bool operator!=(const Vector<T> &lhs, const Vector<T> &rhs)
{
  return !(lhs == rhs);
}

/**
 * @brief Vector print operator
 *
 * @tparam T vector template parameter
 * @param[in, out] ost output stream
 * @param[in] vec vector to print
 * @return std::ostream& modified stream instance
 */
template <std::floating_point T>
std::ostream &operator<<(std::ostream &ost, const Vector<T> &vec)
{
  ost << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
  return ost;
}

using VectorD = Vector<double>;
using VectorF = Vector<float>;

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
bool Vector<T>::isPar(const Vector &rhs) const
{
  return cross(rhs).isEqual(Vector<T>{0});
}

template <std::floating_point T>
bool Vector<T>::isPerp(const Vector &rhs) const
{
  return isNumEq(dot(rhs), 0);
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

} // namespace geom

#endif // __INCLUDE_PRIMITIVES_VECTOR_HH__
