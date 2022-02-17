#ifndef __INCLUDE_PRIMITIVES_TRIANGLE_HH__
#define __INCLUDE_PRIMITIVES_TRIANGLE_HH__

#include <array>
#include <concepts>

#include "vector.hh"

namespace geom
{

template <std::floating_point T>
class Triangle final
{
private:
  std::array<Vector<T>, 3> vertices_;

public:
  Triangle(const Vector<T> &p1, const Vector<T> &p2, const Vector<T> &p3);
  const Vector<T> &operator[](std::size_t idx) const;
};

template <std::floating_point T>
std::ostream &operator<<(std::ostream &ost, const Triangle<T> &tr)
{
  ost << "Triangle: {";
  for (size_t i : {0, 1, 2})
    ost << tr[i] << (i == 2 ? "" : ", ");

  ost << "}";

  return ost;
}

template <std::floating_point T>
Triangle<T>::Triangle(const Vector<T> &p1, const Vector<T> &p2, const Vector<T> &p3)
  : vertices_{p1, p2, p3}
{
}

template <std::floating_point T>
const Vector<T> &Triangle<T>::operator[](std::size_t idx) const
{
  return vertices_[idx % 3];
}

} // namespace geom

#endif // __INCLUDE_PRIMITIVES_TRIANGLE_HH__
