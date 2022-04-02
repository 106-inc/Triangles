#ifndef __INCLUDE_INTERSECTION_INTERSECTION_HH__
#define __INCLUDE_INTERSECTION_INTERSECTION_HH__

#include <cassert>
#include <concepts>
#include <variant>

#include "distance/distance.hh"
#include "primitives/primitives.hh"
#include "primitives/vec2.hh"

#include "detail.hh"

namespace geom
{

/**
 * @brief Checks intersection of 2 triangles
 *
 * @tparam T - floating point type of coordinates
 * @param tr1 first triangle
 * @param tr2 second triangle
 * @return true if triangles are intersect
 * @return false if triangles are not intersect
 */
template <std::floating_point T>
bool isIntersect(const Triangle<T> &tr1, const Triangle<T> &tr2);

/**
 * @brief Intersect 2 planes and return result of intersection
 * @details
 * Common intersection case (parallel planes case is trivial):
 *
 * Let \f$ \overrightarrow{P} \f$ - point in space
 *
 * \f$ pl_1 \f$ equation: \f$ \overrightarrow{n}_1 \cdot \overrightarrow{P} = d_1 \f$
 *
 * \f$ pl_2 \f$ equation: \f$ \overrightarrow{n}_2 \cdot \overrightarrow{P} = d_2 \f$
 *
 * Intersection line direction: \f$ \overrightarrow{dir} = \overrightarrow{n}_1 \times
 * \overrightarrow{n}_2 \f$
 *
 * Let origin of intersection line be a linear combination of \f$ \overrightarrow{n}_1 \f$
 * and \f$ \overrightarrow{n}_2 \f$: \f[ \overrightarrow{P} = a \cdot \overrightarrow{n}_1
 * + b \cdot \overrightarrow{n}_2 \f]
 *
 * \f$ \overrightarrow{P} \f$ must satisfy both \f$ pl_1 \f$ and \f$ pl_1 \f$ equations:
 * \f[
 * \overrightarrow{n}_1 \cdot \overrightarrow{P} = d_1
 * \Leftrightarrow
 * \overrightarrow{n}_1
 * \cdot
 * \left(
 *  a \cdot \overrightarrow{n}_1 + b \cdot \overrightarrow{n}_2
 *  \right)
 *  = d_1
 * \Leftrightarrow
 * a + b \cdot \overrightarrow{n}_1 \cdot \overrightarrow{n}_2  = d_1
 * \f]
 * \f[
 * \overrightarrow{n}_2 \cdot \overrightarrow{P} = d_2
 * \Leftrightarrow
 * \overrightarrow{n}_2
 * \cdot
 * \left(
 *  a \cdot \overrightarrow{n}_1 + b \cdot \overrightarrow{n}_2
 *  \right) = d_2
 * \Leftrightarrow
 * a \cdot \overrightarrow{n}_1 \cdot \overrightarrow{n}_2 + b = d_2
 * \f]
 *
 * Let's find \f$a\f$ and \f$b\f$:
 * \f[
 * a = \frac{
 *  d_2 \cdot \overrightarrow{n}_1 \cdot \overrightarrow{n}_2 - d_1
 * }{
 *  \left( \overrightarrow{n}_1 \cdot \overrightarrow{n}_2\right)^2 - 1
 * }
 * \f]
 * \f[
 * b = \frac{
 * d_1 \cdot \overrightarrow{n}_1 \cdot \overrightarrow{n}_2 - d_2
 * }{
 *  \left( \overrightarrow{n}_1 \cdot \overrightarrow{n}_2\right)^2 - 1
 * }
 * \f]
 *
 * Intersection line equation:
 * \f[
 * \overrightarrow{r}(t) = \overrightarrow{P} + t \cdot \overrightarrow{n}_1 \times
 * \overrightarrow{n}_2 = (a \cdot \overrightarrow{n}_1 + b \cdot \overrightarrow{n}_2) +
 * t \cdot \overrightarrow{n}_1 \times \overrightarrow{n}_2 \f]
 *
 * @tparam T - floating point type of coordinates
 * @param[in] pl1 first plane
 * @param[in] pl2 second plane
 * @return std::variant<std::monostate, Line<T>, Plane<T>>
 */
template <std::floating_point T>
std::variant<std::monostate, Line<T>, Plane<T>> intersect(const Plane<T> &pl1, const Plane<T> &pl2);

/**
 * @brief Intersect 2 lines and return result of intersection
 * @details
 * Common intersection case (parallel & skew lines cases are trivial):
 * Let \f$ \overrightarrow{P} \f$ - point in space, intersection point of two lines.
 *
 * \f$ l_1 \f$ equation: \f$ \overrightarrow{org}_1 + \overrightarrow{dir}_1 \cdot t_1 =
 * \overrightarrow{P} \f$
 *
 * \f$ l_2 \f$ equation: \f$ \overrightarrow{org}_2 + \overrightarrow{dir}_2
 * \cdot t_2 = \overrightarrow{P} \f$
 *
 * Let's equate left sides:
 * \f[
 * \overrightarrow{org}_1 + \overrightarrow{dir}_1 \cdot t_1 =
 * \overrightarrow{org}_2 + \overrightarrow{dir}_2 \cdot t_2
 * \f]
 * Cross multiply both sides from right by \f$ \overrightarrow{dir}_2 \f$:
 * \f[
 * t_1 \cdot \left(\overrightarrow{dir}_1 \times \overrightarrow{dir}_2 \right) =
 * \left(\overrightarrow{org}_2 - \overrightarrow{org}_1 \right) \times \overrightarrow{dir}_2
 * \f]
 * Dot multiply both sides by \f$ \frac{\overrightarrow{dir}_1 \times \overrightarrow{dir}_2}{\left|
 * \overrightarrow{dir}_1 \times \overrightarrow{dir}_2 \right|^2} \f$:
 *
 * \f[
 * t_1 = \frac{
 *  \left(\left(\overrightarrow{org}_2 - \overrightarrow{org}_1 \right) \times
 * \overrightarrow{dir}_2\right) \cdot \left( \overrightarrow{dir}_1 \times \overrightarrow{dir}_2
 * \right)
 * }{
 * \left| \overrightarrow{dir}_1 \times \overrightarrow{dir}_2 \right|^2
 * }
 * \f]
 *
 * Thus we get intersection point parameter \f$ t_1 \f$ on \f$ l_1 \f$, let's substitute it to \f$
 * l_1 \f$ equation: \f[ \overrightarrow{P} = \overrightarrow{org}_1 + \frac{
 *  \left(\left(\overrightarrow{org}_2 - \overrightarrow{org}_1 \right) \times
 * \overrightarrow{dir}_2\right) \cdot \left( \overrightarrow{dir}_1 \times \overrightarrow{dir}_2
 * \right)
 * }{
 * \left| \overrightarrow{dir}_1 \times \overrightarrow{dir}_2 \right|^2
 * } \cdot \overrightarrow{dir}_1
 * \f]
 *
 * @tparam T - floating point type of coordinates
 * @param[in] l1 first line
 * @param[in] l2 second line
 * @return std::variant<std::monostate, Vec3<T>, Line<T>>
 */
template <std::floating_point T>
std::variant<std::monostate, Vec3<T>, Line<T>> intersect(const Line<T> &l1, const Line<T> &l2);

template <std::floating_point T>
bool isIntersect(const Triangle<T> &tr1, const Triangle<T> &tr2)
{
  auto isInv1 = !tr1.isValid();
  auto isInv2 = !tr2.isValid();

  if (isInv1 && isInv2)
    return detail::isIntersectBothInvalid(tr1, tr2);

  if (isInv1)
    return detail::isIntersectValidInvalid(tr2, tr1);

  if (isInv2)
    return detail::isIntersectValidInvalid(tr1, tr2);

  auto pl1 = tr1.getPlane();
  if (detail::isOnOneSide(pl1, tr2))
    return false;

  auto pl2 = tr2.getPlane();
  if (pl1 == pl2)
    return detail::isIntersect2D(tr1, tr2);

  if (pl1.isPar(pl2))
    return false;

  if (detail::isOnOneSide(pl2, tr1))
    return false;

  return detail::isIntersectMollerHaines(tr1, tr2);
}

template <std::floating_point T>
std::variant<std::monostate, Line<T>, Plane<T>> intersect(const Plane<T> &pl1, const Plane<T> &pl2)
{
  const auto &n1 = pl1.norm();
  const auto &n2 = pl2.norm();

  auto dir = cross(n1, n2);

  /* if planes are parallel */
  if (Vec3<T>{0} == dir)
  {
    if (pl1 == pl2)
      return pl1;

    return std::monostate{};
  }

  auto n1n2 = dot(n1, n2);
  auto d1 = pl1.dist();
  auto d2 = pl2.dist();

  auto a = (d2 * n1n2 - d1) / (n1n2 * n1n2 - 1);
  auto b = (d1 * n1n2 - d2) / (n1n2 * n1n2 - 1);

  return Line<T>{(a * n1) + (b * n2), dir};
}

template <std::floating_point T>
std::variant<std::monostate, Vec3<T>, Line<T>> intersect(const Line<T> &l1, const Line<T> &l2)
{
  if (l1.isPar(l2))
  {
    if (l1.isEqual(l2))
      return l1;

    return std::monostate{};
  }

  if (l1.isSkew(l2))
    return std::monostate{};

  auto dir1xdir2 = cross(l1.dir(), l2.dir());
  auto org21xdir2 = cross(l2.org() - l1.org(), l2.dir());

  auto t1_intersect = dot(org21xdir2, dir1xdir2) / dir1xdir2.length2();

  auto point = l1.getPoint(t1_intersect);

  return point;
}

} // namespace geom

#endif // __INCLUDE_INTERSECTION_INTERSECTION_HH__
