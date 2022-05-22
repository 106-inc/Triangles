#ifndef __INCLUDE_PRIMITIVES_LINE_HH__
#define __INCLUDE_PRIMITIVES_LINE_HH__

#include "vec3.hh"

/**
 * @brief line.hh
 * Line class implementation
 */

namespace geom
{

/**
 * @class Line
 * @brief Line class implementation
 *
 * @tparam T - floating point type of coordinates
 */
template <std::floating_point T>
class Line final
{
private:
  /**
   * @brief Origin and direction vectors
   */
  Vec3<T> org_{}, dir_{};

public:
  /**
   * @brief Construct a new Line object
   *
   * @param[in] org origin vector
   * @param[in] dir direction vector
   */
  Line(const Vec3<T> &org, const Vec3<T> &dir);

  /**
   * @brief Line equality operator
   *
   * @tparam T - floating point type of coordinates
   * @param[in] rhs 2nd line
   * @return true if lines are equal
   * @return false if lines are not equal
   */
  bool operator==(const Line &rhs) const;

  /**
   * @brief Line inequality operator
   *
   * @tparam T - floating point type of coordinates
   * @param[in] rhs 2nd line
   * @return true if lines are not equal
   * @return false if lines are equal
   */
  bool operator!=(const Line &rhs) const;

  /**
   * @brief Getter for origin vector
   *
   * @return const Vec3<T>& const reference to origin vector
   */
  const Vec3<T> &org() const &;

  /**
   * @brief Getter for direction vector
   *
   * @return const Vec3<T>& const reference to direction vector
   */
  const Vec3<T> &dir() const &;

  /**
   * @brief Getter for origin vector
   *
   * @return Vec3<T>&& reference to origin vector
   */
  Vec3<T> &&org() &&;

  /**
   * @brief Getter for direction vector
   *
   * @return Vec3<T>&& reference to direction vector
   */
  Vec3<T> &&dir() &&;

  /**
   * @brief Get point on line by parameter t
   *
   * @tparam nType numeric type
   * @param[in] t point paramater from line's equation
   * @return Vec3<T> Point related to parameter
   */
  template <Number nType>
  Vec3<T> getPoint(nType t) const;

  /**
   * @brief Checks is point belongs to line
   *
   * @param[in] point const reference to point vector
   * @return true if point belongs to line
   * @return false if point doesn't belong to line
   */
  bool belongs(const Vec3<T> &point) const;

  /**
   * @brief Checks is *this equals to another line
   *
   * @param[in] line const reference to another line
   * @return true if lines are equal
   * @return false if lines are not equal
   */
  bool isEqual(const Line &line) const;

  /**
   * @brief Checks is *this parallel to another line
   * @note Assumes equal lines as parallel
   * @param[in] line const reference to another line
   * @return true if lines are parallel
   * @return false if lines are not parallel
   */
  bool isPar(const Line &line) const;

  /**
   * @brief Checks is *this is skew with another line
   *
   * @param[in] line const reference to another line
   * @return true if lines are skew
   * @return false if lines are not skew
   */
  bool isSkew(const Line<T> &line) const;

  /**
   * @brief Get line by 2 points
   *
   * @param[in] p1 1st point
   * @param[in] p2 2nd point
   * @return Line passing through two points
   */
  static Line getBy2Points(const Vec3<T> &p1, const Vec3<T> &p2);
};

/**
 * @brief Line print operator
 *
 * @tparam T - floating point type of coordinates
 * @param[in, out] ost output stream
 * @param[in] line Line to print
 * @return std::ostream& modified ostream instance
 */
template <std::floating_point T>
std::ostream &operator<<(std::ostream &ost, const Line<T> &line)
{
  ost << line.org() << " + " << line.dir() << " * t";
  return ost;
}

template <std::floating_point T>
Line<T>::Line(const Vec3<T> &org, const Vec3<T> &dir) : org_{org}, dir_{dir}
{
  if (dir_ == Vec3<T>{0})
    throw std::logic_error{"Direction vector equals zero."};
}

template <std::floating_point T>
bool Line<T>::operator==(const Line &rhs) const
{
  return isEqual(rhs);
}

template <std::floating_point T>
bool Line<T>::operator!=(const Line &rhs) const
{
  return !operator==(rhs);
}

template <std::floating_point T>
const Vec3<T> &Line<T>::org() const &
{
  return org_;
}

template <std::floating_point T>
const Vec3<T> &Line<T>::dir() const &
{
  return dir_;
}

template <std::floating_point T>
Vec3<T> &&Line<T>::org() &&
{
  return std::move(org_);
}

template <std::floating_point T>
Vec3<T> &&Line<T>::dir() &&
{
  return std::move(dir_);
}

template <std::floating_point T>
template <Number nType>
Vec3<T> Line<T>::getPoint(nType t) const
{
  return org_ + dir_ * t;
}

template <std::floating_point T>
bool Line<T>::belongs(const Vec3<T> &point) const
{
  return dir_.cross(point - org_) == Vec3<T>{0};
}

template <std::floating_point T>
bool Line<T>::isEqual(const Line<T> &line) const
{
  return belongs(line.org_) && dir_.isPar(line.dir_);
}

template <std::floating_point T>
bool Line<T>::isPar(const Line<T> &line) const
{
  return dir_.isPar(line.dir_);
}

template <std::floating_point T>
bool Line<T>::isSkew(const Line<T> &line) const
{
  auto res = triple(line.org_ - org_, dir_, line.dir_);
  return !isZeroThreshold(res);
}

template <std::floating_point T>
Line<T> Line<T>::getBy2Points(const Vec3<T> &p1, const Vec3<T> &p2)
{
  return Line<T>{p1, p2 - p1};
}

} // namespace geom

#endif // __INCLUDE_PRIMITIVES_LINE_HH__
