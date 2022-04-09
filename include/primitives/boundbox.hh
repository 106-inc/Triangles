#ifndef __INCLUDE_PRIMITIVES_BOUNDBOX_HH__
#define __INCLUDE_PRIMITIVES_BOUNDBOX_HH__

#include <concepts>

#include "vec3.hh"

namespace geom
{

template <std::floating_point T>
struct BoundBox
{
  T minX{};
  T maxX{};

  T minY{};
  T maxY{};

  T minZ{};
  T maxZ{};
};

template <std::floating_point T>
bool operator==(const BoundBox<T> &lhs, const BoundBox<T> &rhs)
{
  return Vec3<T>::isNumEq(lhs.minX, rhs.minX) && Vec3<T>::isNumEq(lhs.maxX, rhs.maxX) &&
         Vec3<T>::isNumEq(lhs.minY, rhs.minY) && Vec3<T>::isNumEq(lhs.maxY, rhs.maxY) &&
         Vec3<T>::isNumEq(lhs.minZ, rhs.minZ) && Vec3<T>::isNumEq(lhs.maxY, rhs.maxY);
}

} // namespace geom

#endif // __INCLUDE_PRIMITIVES_BOUNDBOX_HH__
