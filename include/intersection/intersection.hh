#ifndef __INCLUDE_INTERSECTION_INTERSECTION_HH__
#define __INCLUDE_INTERSECTION_INTERSECTION_HH__

#include <concepts>

#include "primitives/plane.hh"
#include "primitives/triangle.hh"

namespace geom
{

namespace detail
{
template <std::floating_point T>
bool intersect2D(const Triangle<T> &tr1, const Triangle<T> &tr2)
{
  return false;
}

template <std::floating_point T>
bool intersectMollerHaines(const Triangle<T> &tr1, const Triangle<T> &tr2)
{
  return false;
}
} // namespace detail

template <std::floating_point T>
bool intersect(const Triangle<T> &tr1, const Triangle<T> &tr2)
{
  auto pl1 = Plane<T>::getBy3Points(tr1[0], tr1[1], tr1[2]);
  auto pl2 = Plane<T>::getBy3Points(tr2[0], tr2[1], tr2[2]);

  if (pl1 == pl2)
    return detail::intersect2D(tr1, tr2);

  if (pl1.norm().isPar(pl2.norm()))
    return false;

  return detail::intersectMollerHaines(tr1, tr1);
}

} // namespace geom

#endif // __INCLUDE_INTERSECTION_INTERSECTION_HH__
