#ifndef __INCLUDE_PRIMITIVES_BOUNDBOX_HH__
#define __INCLUDE_PRIMITIVES_BOUNDBOX_HH__

#include <exception>
#include <iostream>

#include "common.hh"
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

  bool belongsTo(const BoundBox<T> &bb);

  T &min(Axis axis) &;
  T &max(Axis axis) &;

  T min(Axis axis) &&;
  T max(Axis axis) &&;

  const T &min(Axis axis) const &;
  const T &max(Axis axis) const &;

  Axis getMaxDim() const;
};

template <std::floating_point T>
bool BoundBox<T>::belongsTo(const BoundBox<T> &bb)
{
  return (minX >= bb.minX) && (minY >= bb.minY) && (minZ >= bb.minZ) && (maxX <= bb.maxX) &&
         (maxY <= bb.maxY) && (maxZ <= bb.maxZ);
}

#define BBFILL(minmax)                                                                             \
  do                                                                                               \
  {                                                                                                \
    switch (axis)                                                                                  \
    {                                                                                              \
    case Axis::X:                                                                                  \
      return minmax##X;                                                                            \
    case Axis::Y:                                                                                  \
      return minmax##Y;                                                                            \
    case Axis::Z:                                                                                  \
      return minmax##Z;                                                                            \
    case Axis::NONE:                                                                               \
    default:                                                                                       \
      throw std::logic_error("BoundBox<T>::" #minmax " (): Wrong input axis");                     \
    }                                                                                              \
  } while (false)

template <std::floating_point T>
T &BoundBox<T>::min(Axis axis) &
{
  BBFILL(min);
}

template <std::floating_point T>
T &BoundBox<T>::max(Axis axis) &
{
  BBFILL(max);
}

template <std::floating_point T>
T BoundBox<T>::min(Axis axis) &&
{
  BBFILL(min);
}

template <std::floating_point T>
T BoundBox<T>::max(Axis axis) &&
{
  BBFILL(max);
}

template <std::floating_point T>
const T &BoundBox<T>::min(Axis axis) const &
{
  BBFILL(min);
}

template <std::floating_point T>
const T &BoundBox<T>::max(Axis axis) const &
{
  BBFILL(max);
}

#undef BBFILL

template <std::floating_point T>
Axis BoundBox<T>::getMaxDim() const
{
  return std::max({Axis::X, Axis::Y, Axis::Z}, [this](const auto &lhs, const auto &rhs) {
    return (this->max(lhs) - this->min(lhs)) < (this->max(rhs) - this->min(rhs));
  });
}

template <std::floating_point T>
bool operator==(const BoundBox<T> &lhs, const BoundBox<T> &rhs)
{
  return Vec3<T>::isNumEq(lhs.minX, rhs.minX) && Vec3<T>::isNumEq(lhs.maxX, rhs.maxX) &&
         Vec3<T>::isNumEq(lhs.minY, rhs.minY) && Vec3<T>::isNumEq(lhs.maxY, rhs.maxY) &&
         Vec3<T>::isNumEq(lhs.minZ, rhs.minZ) && Vec3<T>::isNumEq(lhs.maxY, rhs.maxY);
}

template <std::floating_point T>
std::ostream &operator<<(std::ostream &ost, const BoundBox<T> &bb)
{
  ost << "BB: {\\n";
  ost << "  x: [" << bb.minX << "; " << bb.maxX << "],\\n";
  ost << "  y: [" << bb.minY << "; " << bb.maxY << "],\\n";
  ost << "  z: [" << bb.minZ << "; " << bb.maxZ << "]\\n}";
  return ost;
}

} // namespace geom

#endif // __INCLUDE_PRIMITIVES_BOUNDBOX_HH__
