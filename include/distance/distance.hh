#ifndef __INCLUDE_DISTANCE_DISTANCE_HH__
#define __INCLUDE_DISTANCE_DISTANCE_HH__

#include "primitives/primitives.hh"

namespace geom
{

/**
 * @brief Calculates signed distance between point and plane
 *
 * @tparam T - floating point type of coordinates
 * @param pl plane
 * @param pt point
 * @return T signed distance between point and plane
 */
template <std::floating_point T>
T distance(const Plane<T> &pl, const Vector<T> &pt);

} // namespace geom

namespace geom
{

template <std::floating_point T>
T distance(const Plane<T> &pl, const Vector<T> &pt)
{
  return dot(pt, pl.norm()) - pl.dist();
}

} // namespace geom

#endif // __INCLUDE_DISTANCE_DISTANCE_HH__
