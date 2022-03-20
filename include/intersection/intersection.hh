#ifndef __INCLUDE_INTERSECTION_INTERSECTION_HH__
#define __INCLUDE_INTERSECTION_INTERSECTION_HH__

#include <cassert>
#include <concepts>
#include <variant>

#include "distance/distance.hh"
#include "primitives/primitives.hh"
#include "primitives/vec2.hh"

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

namespace detail
{

template <typename T>
using Segment2D = std::pair<T, T>;

template <std::floating_point T>
using Trian2 = std::array<Vec2<T>, 3>;

template <std::floating_point T>
using Segment3D = std::pair<Vec3<T>, Vec3<T>>;

template <std::floating_point T>
bool isIntersect2D(const Triangle<T> &tr1, const Triangle<T> &tr2);

template <std::floating_point T>
bool isIntersectMollerHaines(const Triangle<T> &tr1, const Triangle<T> &tr2);

template <std::floating_point T>
Segment2D<T> helperMollerHaines(const Triangle<T> &tr, const Plane<T> &pl, const Line<T> &l);

template <std::floating_point T>
bool isIntersectBothInvalid(const Triangle<T> &tr1, const Triangle<T> &tr2);

template <std::floating_point T>
bool isIntersectValidInvalid(const Triangle<T> &valid, const Triangle<T> &invalid);

template <std::floating_point T>
bool isIntersectPointTriangle(const Vec3<T> &pt, const Triangle<T> &tr);

template <std::floating_point T>
bool isIntersectPointSegment(const Vec3<T> &pt, const Segment3D<T> &segm);

template <std::floating_point T>
bool isIntersectSegmentSegment(const Segment3D<T> &segm1, const Segment3D<T> &segm2);

template <std::floating_point T>
bool isPoint(const Triangle<T> &tr);

template <std::floating_point T>
bool isOverlap(Segment2D<T> &segm1, Segment2D<T> &segm2);

template <std::forward_iterator It>
bool isSameSign(It begin, It end);

template <std::floating_point T>
bool isOnOneSide(const Plane<T> &pl, const Triangle<T> &tr);

template <std::floating_point T>
Trian2<T> getTrian2(const Plane<T> &pl, const Triangle<T> &tr);

template <std::floating_point T>
bool isCounterClockwise(Trian2<T> &tr);

template <std::floating_point T>
Segment2D<T> computeInterval(const Trian2<T> &tr, const Vec2<T> &d);

template <std::floating_point T>
Segment3D<T> getSegment(const Triangle<T> &tr);

} // namespace detail
} // namespace geom

namespace geom
{

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

namespace detail
{

template <std::floating_point T>
bool isIntersect2D(const Triangle<T> &tr1, const Triangle<T> &tr2)
{
  auto pl = tr1.getPlane();

  auto trian1 = getTrian2(pl, tr1);
  auto trian2 = getTrian2(pl, tr2);

  for (auto trian : {trian1, trian2})
  {
    for (size_t i0 = 0, i1 = 2; i0 < 3; i1 = i0, ++i0)
    {
      auto d = (trian[i0] - trian[i1]).getPerp();

      auto s1 = computeInterval(trian1, d);
      auto s2 = computeInterval(trian2, d);

      if (s2.second < s1.first || s1.second < s2.first)
        return false;
    }
  }

  return true;
}

template <std::floating_point T>
bool isIntersectMollerHaines(const Triangle<T> &tr1, const Triangle<T> &tr2)
{
  auto pl1 = tr1.getPlane();
  auto pl2 = tr2.getPlane();

  auto l = std::get<Line<T>>(intersect(pl1, pl2));

  auto params1 = helperMollerHaines(tr1, pl2, l);
  auto params2 = helperMollerHaines(tr2, pl1, l);

  return isOverlap(params1, params2);
}

template <std::floating_point T>
Segment2D<T> helperMollerHaines(const Triangle<T> &tr, const Plane<T> &pl, const Line<T> &l)
{
  /* Project the triangle vertices onto line */
  std::array<T, 3> vert{};
  for (size_t i = 0; i < 3; ++i)
    vert[i] = dot(l.dir(), tr[i] - l.org());

  std::array<T, 3> sdist{};
  for (size_t i = 0; i < 3; ++i)
    sdist[i] = distance(pl, tr[i]);

  auto isSameSign = [](const auto &num1, const auto &num2) {
    if (num1 * num2 > Vec3<T>::getThreshold())
      return true;
    return Vec3<T>::isNumEq(num1, 0) && Vec3<T>::isNumEq(num2, 0);
  };

  std::array<bool, 3> isOneSide{};
  for (size_t i = 0; i < 3; ++i)
    isOneSide[i] = isSameSign(sdist[i], sdist[(i + 1) % 3]);

  /* Looking for vertex which is alone on it's side */
  size_t rogue = 0;
  for (size_t i = 0; i < 3; ++i)
    if (isOneSide[i])
      rogue = (i + 2) % 3;

  std::vector<T> segm{};
  std::array<size_t, 2> arr{(rogue + 1) % 3, (rogue + 2) % 3};

  for (size_t i : arr)
    segm.push_back(vert[i] + (vert[rogue] - vert[i]) * sdist[i] / (sdist[i] - sdist[rogue]));

  /* Sort segment's ends */
  if (segm[0] > segm[1])
    std::swap(segm[0], segm[1]);

  return {segm[0], segm[1]};
}

template <std::floating_point T>
bool isIntersectBothInvalid(const Triangle<T> &tr1, const Triangle<T> &tr2)
{
  auto isPoint1 = isPoint(tr1);
  auto isPoint2 = isPoint(tr2);

  if (isPoint1 && isPoint2)
    return tr1[0] == tr2[0];

  if (isPoint1)
    return isIntersectPointSegment(tr1[0], getSegment(tr2));

  if (isPoint2)
    return isIntersectPointSegment(tr2[0], getSegment(tr1));

  return isIntersectSegmentSegment(getSegment(tr1), getSegment(tr2));
}

template <std::floating_point T>
bool isIntersectValidInvalid(const Triangle<T> &valid, const Triangle<T> &invalid)
{
  if (isPoint(invalid))
    return isIntersectPointTriangle(invalid[0], valid);

  auto segm = getSegment(invalid);
  auto pl = valid.getPlane();

  auto dst1 = distance(pl, segm.first);
  auto dst2 = distance(pl, segm.second);

  if (dst1 * dst2 > 0)
    return false;

  if (Vec3<T>::isNumEq(dst1, 0) && Vec3<T>::isNumEq(dst2, 0))
    return isIntersect2D(valid, invalid);

  dst1 = std::abs(dst1);
  dst2 = std::abs(dst2);

  auto pt = segm.first + (segm.second - segm.first) * dst1 / (dst1 + dst2);
  return isIntersectPointTriangle(pt, valid);
}

template <std::floating_point T>
bool isIntersectPointTriangle(const Vec3<T> &pt, const Triangle<T> &tr)
{
  if (!tr.getPlane().belongs(pt))
    return false;

  /* TODO: comment better */
  /* pt = point + u * edge1 + v * edge2 */
  auto point = pt - tr[0];
  auto edge1 = tr[1] - tr[0];
  auto edge2 = tr[2] - tr[0];

  auto dotE1E1 = dot(edge1, edge1);
  auto dotE1E2 = dot(edge1, edge2);
  auto dotE1PT = dot(edge1, point);

  auto dotE2E2 = dot(edge2, edge2);
  auto dotE2PT = dot(edge2, point);

  auto denom = dotE1E1 * dotE2E2 - dotE1E2 * dotE1E2;
  auto u = (dotE2E2 * dotE1PT - dotE1E2 * dotE2PT) / denom;
  auto v = (dotE1E1 * dotE2PT - dotE1E2 * dotE1PT) / denom;

  /* Point belongs to triangle if: (u >= 0) && (v >= 0) && (u + v <= 1) */
  auto eps = Vec3<T>::getThreshold();
  return (u > -eps) && (v > -eps) && (u + v < 1 + eps);
}

template <std::floating_point T>
bool isIntersectPointSegment(const Vec3<T> &pt, const Segment3D<T> &segm)
{
  Line<T> l{segm.first, segm.second - segm.first};
  if (!l.belongs(pt))
    return false;

  auto beg = dot(l.dir(), segm.first);
  auto end = dot(l.dir(), segm.second);
  auto proj = dot(l.dir(), pt);

  return !((proj > end) || (proj < beg));
}

template <std::floating_point T>
bool isIntersectSegmentSegment(const Segment3D<T> &segm1, const Segment3D<T> &segm2)
{
  Line<T> l1{segm1.first, segm1.second - segm1.first};
  Line<T> l2{segm2.first, segm2.second - segm2.first};
  auto intersectionResult = intersect(l1, l2);

  if (std::holds_alternative<Line<T>>(intersectionResult))
  {
    const auto &dir = l1.dir();
    Segment2D<T> s1{dot(dir, segm1.first), dot(dir, segm1.second)};
    Segment2D<T> s2{dot(dir, segm2.first), dot(dir, segm2.second)};
    return isOverlap(s1, s2);
  }

  if (std::holds_alternative<Vec3<T>>(intersectionResult))
  {
    auto pt = std::get<Vec3<T>>(intersectionResult);
    return isIntersectPointSegment(pt, segm1) && isIntersectPointSegment(pt, segm2);
  }

  return false;
}

template <std::floating_point T>
bool isPoint(const Triangle<T> &tr)
{
  return (tr[0] == tr[1]) && (tr[0] == tr[2]);
}

template <std::floating_point T>
bool isOverlap(Segment2D<T> &segm1, Segment2D<T> &segm2)
{
  return (segm2.first <= segm1.second) && (segm2.second >= segm1.first);
}

template <std::forward_iterator It>
bool isSameSign(It begin, It end)
{
  auto cur = begin;
  auto prev = begin;

  for (++cur; cur != end; ++cur)
    if ((*cur) * (*prev) <= 0)
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
    return true;

  return false;
}

template <std::floating_point T>
Trian2<T> getTrian2(const Plane<T> &pl, const Triangle<T> &tr)
{
  auto norm = pl.norm();

  const Vec3<T> x{1, 0, 0};
  const Vec3<T> y{0, 1, 0};
  const Vec3<T> z{0, 0, 1};

  std::array<Vec3<T>, 3> xyz{x, y, z};
  std::array<T, 3> xyzDot;

  std::transform(xyz.begin(), xyz.end(), xyzDot.begin(),
                 [&norm](const auto &axis) { return std::abs(dot(axis, norm)); });

  auto maxIt = std::max_element(xyzDot.begin(), xyzDot.end());
  auto maxIdx = static_cast<size_t>(maxIt - xyzDot.begin());

  Trian2<T> res;
  for (size_t i = 0; i < 3; ++i)
    for (size_t j = 0, k = 0; j < 2; ++j, ++k)
    {
      if (k == maxIdx)
        ++k;

      res[i][j] = tr[i][k];
    }

  if (!isCounterClockwise(res))
    std::swap(res[0], res[1]);

  return res;
}

template <std::floating_point T>
bool isCounterClockwise(Trian2<T> &tr)
{
  /**
   * The triangle is counterclockwise ordered if \delta > 0
   * and clockwise ordered if \delta < 0.
   *
   *              +  1  1  1 +
   * \delta = det | x0 x1 x2 | = (x1 * y2 - x2 * y1) - (x0 * y2 - x2 * y0)
   *              + y0 y1 y2 +                       + (x0 * y1 - x1 * y0)
   *
   */

  auto x0 = tr[0][0], x1 = tr[1][0], x2 = tr[2][0];
  auto y0 = tr[0][1], y1 = tr[1][1], y2 = tr[2][1];

  auto delta = (x1 * y2 - x2 * y1) - (x0 * y2 - x2 * y0) + (x0 * y1 - x1 * y0);
  return (delta > 0);
}

template <std::floating_point T>
Segment2D<T> computeInterval(const Trian2<T> &tr, const Vec2<T> &d)
{
  auto init = dot(d, tr[0]);
  auto min = init;
  auto max = init;

  for (size_t i = 1; i < 3; ++i)
    if (auto val = dot(d, tr[i]); val < min)
      min = val;
    else if (val > max)
      max = val;

  return {min, max};
}

template <std::floating_point T>
Segment3D<T> getSegment(const Triangle<T> &tr)
{
  std::array<T, 3> lenArr{};
  for (size_t i = 0; i < 3; ++i)
    lenArr[i] = (tr[i] - tr[i + 1]).length2();

  auto maxIt = std::max_element(lenArr.begin(), lenArr.end());
  auto maxIdx = static_cast<size_t>(maxIt - lenArr.begin());

  return {tr[maxIdx], tr[maxIdx + 1]};
}

} // namespace detail
} // namespace geom

#endif // __INCLUDE_INTERSECTION_INTERSECTION_HH__
