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
 * @param pl1 first plane
 * @param pl2 second plane
 * @return std::variant<std::monostate, Line<T>, Plane<T>>
 */
template <std::floating_point T>
std::variant<std::monostate, Line<T>, Plane<T>> intersect(const Plane<T> &pl1,
                                                          const Plane<T> &pl2);

namespace detail
{

template <typename T>
using Segment = std::pair<T, T>;

template <std::floating_point T>
bool isIntersect2D(const Triangle<T> &tr1, const Triangle<T> &tr2);

template <std::floating_point T>
bool isIntersectMollerHaines(const Triangle<T> &tr1, const Triangle<T> &tr2);

template <std::floating_point T>
Segment<T> helperMollerHaines(const Triangle<T> &tr, const Plane<T> &pl,
                              const Line<T> &l);

template <std::floating_point T>
bool isOverlap(Segment<T> &segm1, Segment<T> &segm2);

template <std::forward_iterator It>
bool isSameSign(It begin, It end);

template <std::floating_point T>
bool isOnOneSide(const Plane<T> &pl, const Triangle<T> &tr);

} // namespace detail
} // namespace geom

namespace geom
{

template <std::floating_point T>
bool isIntersect(const Triangle<T> &tr1, const Triangle<T> &tr2)
{
  /* TODO: handle invalid triangles case */

  auto pl1 = Plane<T>::getBy3Points(tr1[0], tr1[1], tr1[2]);

  if (!detail::isOnOneSide(pl1, tr2))
    return false;

  auto pl2 = Plane<T>::getBy3Points(tr2[0], tr2[1], tr2[2]);

  if (pl1 == pl2)
    return detail::isIntersect2D(tr1, tr2);

  if (pl1.isPar(pl2))
    return false;

  if (!detail::isOnOneSide(pl2, tr1))
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

  auto pl1 = Plane<T>::getBy3Points(tr1[0], tr1[1], tr1[2]);
  auto pl2 = Plane<T>::getBy3Points(tr2[0], tr2[1], tr2[2]);

  auto l = std::get<Line<T>>(intersect(pl1, pl2));

  auto params1 = helperMollerHaines(tr1, pl2, l);
  auto params2 = helperMollerHaines(tr2, pl1, l);

  return isOverlap(params1, params2);
}

template <std::floating_point T>
Segment<T> helperMollerHaines(const Triangle<T> &tr, const Plane<T> &pl, const Line<T> &l)
{
  /* Project the triangle vertices onto line */
  std::array<T, 3> vert{};
  for (size_t i = 0; i < 3; ++i)
    vert[i] = dot(l.dir(), tr[i] - l.org());

  std::array<T, 3> sdist{};
  for (size_t i = 0; i < 3; ++i)
    sdist[i] = distance(pl, tr[i]);

  std::array<bool, 3> isOneSide{};
  for (size_t i = 0; i < 3; ++i)
    isOneSide[i] = (sdist[i] * sdist[(i + 1) % 3] > 0);

  /* Looking for vertex which is alone on it's side */
  size_t rogue = 0;
  for (size_t i = 0; i < 3; ++i)
    if (isOneSide[i])
      rogue = (i + 2) % 3;

  std::vector<T> segm{};
  std::array<size_t, 2> arr{(rogue + 1) % 3, (rogue + 2) % 3};

  for (size_t i : arr)
    segm.push_back(vert[i] +
                   (vert[rogue] - vert[i]) * sdist[i] / (sdist[i] - sdist[rogue]));

  /* Sort segment's ends */
  if (segm[0] > segm[1])
    std::swap(segm[0], segm[1]);

  return {segm[0], segm[1]};
}

template <std::floating_point T>
bool isOverlap(Segment<T> &segm1, Segment<T> &segm2)
{
  return (segm2.first <= segm1.second) && (segm2.second >= segm1.first);
}

template <std::forward_iterator It>
bool isSameSign(It begin, It end)
{
  auto cur = begin;
  auto prev = begin;

  for (++cur; cur != end; ++cur)
    if ((*cur) * (*prev) < 0)
      return false;

  return true;
}

template <std::floating_point T>
bool isOnOneSide(const Plane<T> &pl, const Triangle<T> &tr)
{
  std::array<T, 3> sdist{};
  for (size_t i = 0; i < 3; ++i)
    sdist[i] = distance(pl, tr[i]);

  if (detail::isSameSign(sdist.begin(), sdist.end()))
    return false;

  return true;
}

} // namespace detail
} // namespace geom

#endif // __INCLUDE_INTERSECTION_INTERSECTION_HH__
