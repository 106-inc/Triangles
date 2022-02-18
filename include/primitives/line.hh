#ifndef __INCLUDE_PRIMITIVES_LINE_HH__
#define __INCLUDE_PRIMITIVES_LINE_HH__

#include "vector.hh"

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
  Vector<T> org_{}, dir_{};

public:
  /**
   * @brief Construct a new Line object
   *
   * @param[in] org origin vector
   * @param[in] dir direction vector
   */
  Line(const Vector<T> &org, const Vector<T> &dir);

  /**
   * @brief Getter for origin vector
   *
   * @return const Vector<T>& const reference to origin vector
   */
  const Vector<T> &org() const;

  /**
   * @brief Getter for direction vector
   *
   * @return const Vector<T>& const reference to direction vector
   */
  const Vector<T> &dir() const;

  /**
   * @brief Checks is point belongs to line
   *
   * @param[in] point const reference to point vector
   * @return true if point belongs to line
   * @return false if point doesn't belong to line
   */
  bool belongs(const Vector<T> &point) const;

  /**
   * @brief Checks is *this equals to another line
   *
   * @param[in] line const reference to another line
   * @return true if lines are equal
   * @return false if lines are not equal
   */
  bool isEqual(const Line &line) const;

  /**
   * @brief Get line by 2 points
   *
   * @param[in] p1 1st point
   * @param[in] p2 2nd point
   * @return Line passing through two points
   */
  static Line getBy2Points(const Vector<T> &p1, const Vector<T> &p2);
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

/**
 * @brief Line equality operator
 *
 * @tparam T - floating point type of coordinates
 * @param[in] lhs 1st line
 * @param[in] rhs 2nd line
 * @return true if lines are equal
 * @return false if lines are not equal
 */
template <std::floating_point T>
bool operator==(const Line<T> &lhs, const Line<T> &rhs)
{
  return lhs.isEqual(rhs);
}

template <std::floating_point T>
Line<T>::Line(const Vector<T> &org, const Vector<T> &dir) : org_{org}, dir_{dir}
{
  if (dir_ == Vector<T>{0})
    throw std::logic_error{"Direction vector equals zero."};
}

template <std::floating_point T>
const Vector<T> &Line<T>::org() const
{
  return org_;
}

template <std::floating_point T>
const Vector<T> &Line<T>::dir() const
{
  return dir_;
}

template <std::floating_point T>
bool Line<T>::belongs(const Vector<T> &point) const
{
  return dir_.cross(point - org_) == Vector<T>{0};
}

template <std::floating_point T>
bool Line<T>::isEqual(const Line<T> &line) const
{
  return belongs(line.org_) && dir_.isPar(line.dir_);
}

template <std::floating_point T>
Line<T> Line<T>::getBy2Points(const Vector<T> &p1, const Vector<T> &p2)
{
  return Line<T>{p1, p2 - p1};
}

} // namespace geom

#endif // __INCLUDE_PRIMITIVES_LINE_HH__
