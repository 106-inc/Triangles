#ifndef __INCLUDE_INTERSECTION_INTERSECTION_HH__
#define __INCLUDE_INTERSECTION_INTERSECTION_HH__

#include <concepts>
#include <variant>

#include "primitives/plane.hh"
#include "primitives/triangle.hh"

namespace geom
{

template <std::floating_point T>
std::variant<std::monostate, Line<T>, Plane<T>> intersect(const Plane<T> &pl1,
                                                          const Plane<T> &pl2)
{
  const auto &n1 = pl1.norm();
  const auto &n2 = pl2.norm();

  auto dir = cross(n1, n2);

  if (Vector<T>{0} == dir)
  {
    if (pl1.isEqual(pl2))
      return pl1;

    return std::monostate{};
  }

  return std::monostate{};
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
  assert(false && "Not implemented yet");
  return false;
}
} // namespace detail

template <std::floating_point T>
bool isIntersect(const Triangle<T> &tr1, const Triangle<T> &tr2)
{
  auto pl1 = Plane<T>::getBy3Points(tr1[0], tr1[1], tr1[2]);
  auto pl2 = Plane<T>::getBy3Points(tr2[0], tr2[1], tr2[2]);

  if (pl1 == pl2)
    return detail::isIntersect2D(tr1, tr2);

  if (pl1.isPar(pl2))
    return false;

  return detail::isIntersectMollerHaines(tr1, tr1);
}

} // namespace geom

#endif // __INCLUDE_INTERSECTION_INTERSECTION_HH__
