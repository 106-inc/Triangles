#ifndef __INCLUDE_INTERSECTION_INTERSECTION_HH__
#define __INCLUDE_INTERSECTION_INTERSECTION_HH__

#include <concepts>
#include <variant>

#include "primitives/plane.hh"
#include "primitives/triangle.hh"

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
bool isIntersect2D(const Triangle<T> &tr1, const Triangle<T> &tr2);

/**
 * @brief Intersect 2 planes and return result of intersection
 * @details
 * Common intersection case (parallel planes case is trivial):
 *
 * Let P = point in space
 *
 * pl1 equation: dot(n1, P) = d1
 * pl2 equation: dot(n2, P) = d2
 *
 * Intersection line direction: dir = cross(n1, n2)
 *
 * Let origin of intersection line be a linear combination of n1 and n2:
 * P = a * n1 + b * n2
 *
 * P must satisfy both pl1 and pl2 equations:
 * dot(n1, P) = d1 <=> dot(n1, a * n1 + b * n2) = d1 <=> a + b * dot(n1, n2) = d1
 * dot(n2, P) = d2 <=> dot(n2, a * n1 + b * n2) = d2 <=> a * dot(n1, n2) + b = d2
 *
 * Let's find a and b:
 * a = (d2 * dot(n1, n2) - d1) / ((dot(n1, n2))^2 - 1)
 * b = (d1 * dot(n1, n2) - d2) / ((dot(n1, n2))^2 - 1)
 *
 * Intersection line equation:
 * L = P + t * cross(n1, n2) = (a * n1 + b * n2) + t * cross(n1, n2)
 *
 * @tparam T - floating point type of coordinates
 * @param pl1 first plane
 * @param pl2 second plane
 * @return std::variant<std::monostate, Line<T>, Plane<T>>
 */
template <std::floating_point T>
std::variant<std::monostate, Line<T>, Plane<T>> intersect(const Plane<T> &pl1,
                                                          const Plane<T> &pl2);

namespace detail
{

template <std::floating_point T>
bool isIntersect2D(const Triangle<T> &tr1, const Triangle<T> &tr2);

template <std::floating_point T>
bool isIntersectMollerHaines(const Triangle<T> &tr1, const Triangle<T> &tr2);

} // namespace detail
} // namespace geom

namespace geom
{

template <std::floating_point T>
bool isIntersect(const Triangle<T> &tr1, const Triangle<T> &tr2)
{
  /* TODO: handle invalid triangles case */
  auto pl1 = Plane<T>::getBy3Points(tr1[0], tr1[1], tr1[2]);
  auto pl2 = Plane<T>::getBy3Points(tr2[0], tr2[1], tr2[2]);

  if (pl1 == pl2)
    return detail::isIntersect2D(tr1, tr2);

  if (pl1.isPar(pl2))
    return false;

  return detail::isIntersectMollerHaines(tr1, tr1);
}

template <std::floating_point T>
std::variant<std::monostate, Line<T>, Plane<T>> intersect(const Plane<T> &pl1,
                                                          const Plane<T> &pl2)
{
  const auto &n1 = pl1.norm();
  const auto &n2 = pl2.norm();

  auto dir = cross(n1, n2);

  /* if planes are parallel */
  if (Vector<T>{0} == dir)
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

namespace detail
{

template <std::floating_point T>
bool isIntersect2D(const Triangle<T> &tr1, const Triangle<T> &tr2)
{
  assert(false && "Not implemented yet");
  return false;
}

template <std::floating_point T>
bool isIntersectMollerHaines(const Triangle<T> &tr1, const Triangle<T> &tr2)
{
  auto pl1 = Plane<T>::getBy3Points(tr1[0], tr1[1], tr1[2]);
  auto pl2 = Plane<T>::getBy3Points(tr2[0], tr2[1], tr2[2]);

  auto l = std::get<Line<T>>(intersect(pl1, pl2));

  /* Project the triangle vertices onto line */
  std::array<T, 3> vert1, vert2;
  for (size_t i = 0; i < 3; ++i)
  {
    vert1[i] = dot(l.dir(), tr1[i] - l.org());
    vert2[i] = dot(l.dir(), tr2[i] - l.org());
  }

  /* TODO: get and compare parameters */
  assert(false && "Not implemented yet");
  return false;
}
} // namespace detail
} // namespace geom

#endif // __INCLUDE_INTERSECTION_INTERSECTION_HH__
