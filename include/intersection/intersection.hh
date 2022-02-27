#ifndef __INCLUDE_INTERSECTION_INTERSECTION_HH__
#define __INCLUDE_INTERSECTION_INTERSECTION_HH__

#include <concepts>
#include <variant>

#include "distance/distance.hh"
#include "primitives/primitives.hh"

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

template <std::floating_point T>
bool isOverlap(const T &params10, const T &params11, const T &params20,
               const T &params21);

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

  return detail::isIntersectMollerHaines(tr1, tr2);
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
  // All this function is HARDCODE
  // TODO:
  // 1) make it more beautiful
  // 2) add handling such case: all points of triangle has signed dist to plane of another
  // triangle of same sign

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

  std::array<T, 3> sdist1, sdist2;
  for (size_t i = 0; i < 3; ++i)
  {
    sdist1[i] = distance(pl2, tr1[i]);
    sdist2[i] = distance(pl1, tr2[i]);
  }

  std::array<T, 3> sign1, sign2;
  for (size_t i = 0; i < 3; ++i)
  {
    sign1[i] = sdist1[i] * sdist1[(i + 1) % 3];
    sign2[i] = sdist2[i] * sdist2[(i + 1) % 3];
  }

  size_t rogue1 = 0;
  size_t rogue2 = 0;
  for (size_t i = 0; i < 3; ++i)
  {
    if (sign1[i] >= 0)
      rogue1 = (i + 2) % 3;

    if (sign2[i] >= 0)
      rogue2 = (i + 2) % 3;
  }

  std::vector<T> params1, params2;
  std::array<size_t, 2> arr1{(rogue1 + 1) % 3, (rogue1 + 2) % 3};
  std::array<size_t, 2> arr2{(rogue2 + 1) % 3, (rogue2 + 2) % 3};

  for (size_t i : arr1)
    params1.push_back(vert1[i] + (vert1[rogue1] - vert1[i]) * sdist1[i] /
                                   (sdist1[i] - sdist1[rogue1]));

  for (size_t i : arr2)
    params2.push_back(vert2[i] + (vert2[rogue2] - vert2[i]) * sdist2[i] /
                                   (sdist2[i] - sdist2[rogue2]));

  std::sort(params1.begin(), params1.end());
  std::sort(params2.begin(), params2.end());

  return isOverlap(params1[0], params1[1], params2[0], params2[1]);
}

template <std::floating_point T>
bool isOverlap(const T &params10, const T &params11, const T &params20, const T &params21)
{
  return (params20 <= params11) && (params21 >= params10);
}

} // namespace detail
} // namespace geom

#endif // __INCLUDE_INTERSECTION_INTERSECTION_HH__
