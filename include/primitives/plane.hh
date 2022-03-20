#ifndef __INCLUDE_PRIMITIVES_PLANE_HH__
#define __INCLUDE_PRIMITIVES_PLANE_HH__

#include "line.hh"
#include "vec3.hh"

/**
 * @brief
 * Plane class implementation
 */

namespace geom
{

/**
 * @class Plane
 * @brief Plane class realization
 *
 * @tparam T - floating point type of coordinates
 */
template <std::floating_point T>
class Plane final
{
private:
  /**
   * @brief Normal vector, length equals to 1
   */
  Vec3<T> norm_{};

  /**
   * @brief Distance from zero to plane
   */
  T dist_{};

  /**
   * @brief Construct a new Plane object from normal vector and distance
   *
   * @param[in] norm normal vector
   * @param[in] dist distance from plane to zero
   */
  Plane(const Vec3<T> &norm, T dist);

public:
  /**
   * @brief Getter for distance
   *
   * @return T value of distance
   */
  T dist() const;

  /**
   * @brief Getter for normal vector
   *
   * @return const Vec3<T>& const reference to normal vector
   */
  const Vec3<T> &norm() const;

  /**
   * @brief Checks if point belongs to plane
   *
   * @param[in] point const referene to point vector
   * @return true if point belongs to plane
   * @return false if point doesn't belong to plane
   */
  bool belongs(const Vec3<T> &point) const;

  /**
   * @brief Checks if line belongs to plane
   *
   * @param[in] line const referene to line
   * @return true if line belongs to plane
   * @return false if line doesn't belong to plane
   */
  bool belongs(const Line<T> &line) const;

  /**
   * @brief Checks is *this equals to another plane
   *
   * @param[in] rhs const reference to another plane
   * @return true if planes are equal
   * @return false if planes are not equal
   */
  bool isEqual(const Plane &rhs) const;

  /**
   * @brief Checks is *this is parallel to another plane
   *
   * @param[in] rhs const reference to another plane
   * @return true if planes are parallel
   * @return false if planes are not parallel
   */
  bool isPar(const Plane &rhs) const;

  /**
   * @brief Get plane by 3 points
   *
   * @param[in] pt1 1st point
   * @param[in] pt2 2nd point
   * @param[in] pt3 3rd point
   * @return Plane passing through three points
   */
  static Plane getBy3Points(const Vec3<T> &pt1, const Vec3<T> &pt2, const Vec3<T> &pt3);

  /**
   * @brief Get plane from parametric plane equation
   *
   * @param[in] org origin vector
   * @param[in] dir1 1st direction vector
   * @param[in] dir2 2nd direction vector
   * @return Plane
   */
  static Plane getParametric(const Vec3<T> &org, const Vec3<T> &dir1, const Vec3<T> &dir2);

  /**
   * @brief Get plane from normal point plane equation
   *
   * @param[in] norm normal vector
   * @param[in] point point lying on the plane
   * @return Plane
   */
  static Plane getNormalPoint(const Vec3<T> &norm, const Vec3<T> &point);

  /**
   * @brief Get plane form normal const plane equation
   *
   * @param[in] norm normal vector
   * @param[in] constant distance
   * @return Plane
   */
  static Plane getNormalDist(const Vec3<T> &norm, T constant);
};

/**
 * @brief Plane equality operator
 *
 * @tparam T - floating point type of coordinates
 * @param[in] lhs 1st plane
 * @param[in] rhs 2nd plane
 * @return true if planes are equal
 * @return false if planes are not equal
 */
template <std::floating_point T>
bool operator==(const Plane<T> &lhs, const Plane<T> &rhs)
{
  return lhs.isEqual(rhs);
}

/**
 * @brief Plane print operator
 *
 * @tparam T - floating point type of coordinates
 * @param[in, out] ost output stream
 * @param[in] pl plane to print
 * @return std::ostream& modified ostream instance
 */
template <std::floating_point T>
std::ostream &operator<<(std::ostream &ost, const Plane<T> &pl)
{
  ost << pl.norm() << " * X = " << pl.dist();
  return ost;
}

template <std::floating_point T>
Plane<T>::Plane(const Vec3<T> &norm, T dist) : norm_(norm), dist_(dist)
{
  if (norm == Vec3<T>{0})
    throw std::logic_error{"normal vector equals to zero"};
}

template <std::floating_point T>
T Plane<T>::dist() const
{
  return dist_;
}

template <std::floating_point T>
const Vec3<T> &Plane<T>::norm() const
{
  return norm_;
}

template <std::floating_point T>
bool Plane<T>::belongs(const Vec3<T> &pt) const
{
  return Vec3<T>::isNumEq(norm_.dot(pt), dist_);
}

template <std::floating_point T>
bool Plane<T>::belongs(const Line<T> &line) const
{
  return norm_.isPerp(line.dir()) && belongs(line.org());
}

template <std::floating_point T>
bool Plane<T>::isEqual(const Plane &rhs) const
{
  return (norm_ * dist_ == rhs.norm_ * rhs.dist_) && (norm_.isPar(rhs.norm_));
}

template <std::floating_point T>
bool Plane<T>::isPar(const Plane &rhs) const
{
  return norm_.isPar(rhs.norm_);
}

template <std::floating_point T>
Plane<T> Plane<T>::getBy3Points(const Vec3<T> &pt1, const Vec3<T> &pt2, const Vec3<T> &pt3)
{
  return getParametric(pt1, pt2 - pt1, pt3 - pt1);
}

template <std::floating_point T>
Plane<T> Plane<T>::getParametric(const Vec3<T> &org, const Vec3<T> &dir1, const Vec3<T> &dir2)
{
  auto norm = dir1.cross(dir2);
  return getNormalPoint(norm, org);
}

template <std::floating_point T>
Plane<T> Plane<T>::getNormalPoint(const Vec3<T> &norm, const Vec3<T> &pt)
{
  auto normalized = norm.normalized();
  return Plane{normalized, normalized.dot(pt)};
}

template <std::floating_point T>
Plane<T> Plane<T>::getNormalDist(const Vec3<T> &norm, T dist)
{
  auto normalized = norm.normalized();
  return Plane{normalized, dist};
}

} // namespace geom

#endif // __INCLUDE_PRIMITIVES_PLANE_HH__
