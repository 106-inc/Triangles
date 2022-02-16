#ifndef __INCLUDE_PRIMITIVES_PLANE_HH__
#define __INCLUDE_PRIMITIVES_PLANE_HH__

#include <iostream>

#include "line.hh"
#include "vector.hh"

namespace geom
{

template <std::floating_point T>
class Plane final
{
private:
  Vector<T> norm_{}; // normal vector, length equals to 1
  T dist_{};         // distance from zero to plane

  Plane(const Vector<T> &norm, T dist);

public:
  T dist() const;
  const Vector<T> &norm() const;

  bool belongs(const Vector<T> &point) const;
  bool belongs(const Line<T> &line) const;
  bool isEqual(const Plane &rhs) const;

  static Plane getBy3Points(const Vector<T> &p1, const Vector<T> &p2,
                            const Vector<T> &p3);
  static Plane getParametric(const Vector<T> &org, const Vector<T> &dir1,
                             const Vector<T> &dir2);
  static Plane getNormalPoint(const Vector<T> &norm, const Vector<T> &point);
  static Plane getNormalDist(const Vector<T> &norm, T constant);
};

template <std::floating_point T>
Plane<T>::Plane(const Vector<T> &norm, T dist) : norm_(norm), dist_(dist)
{
  if (norm == Vector<T>{0})
    throw std::logic_error{"normal vector equals to zero"};
}

template <std::floating_point T>
T Plane<T>::dist() const
{
  return dist_;
}

template <std::floating_point T>
const Vector<T> &Plane<T>::norm() const
{
  return norm_;
}

template <std::floating_point T>
bool Plane<T>::belongs(const Vector<T> &point) const
{
  return Vector<T>::isNumEq(norm_.dot(point), dist_);
}

template <std::floating_point T>
bool Plane<T>::belongs(const Line<T> &line) const
{
  return norm_.isPerp(line.dir()) && belongs(line.org());
}

template <std::floating_point T>
bool Plane<T>::isEqual(const Plane &rhs) const
{
  return (norm_ * dist_ == rhs.norm_ * rhs.dist_) && (norm_.isPar(rhs.norm_));
}

template <std::floating_point T>
Plane<T> Plane<T>::getBy3Points(const Vector<T> &p1, const Vector<T> &p2,
                                const Vector<T> &p3)
{
  return getParametric(p1, p2 - p1, p3 - p1);
}

template <std::floating_point T>
Plane<T> Plane<T>::getParametric(const Vector<T> &org, const Vector<T> &dir1,
                                 const Vector<T> &dir2)
{
  auto norm = dir1.cross(dir2);
  return getNormalPoint(norm, org);
}

template <std::floating_point T>
Plane<T> Plane<T>::getNormalPoint(const Vector<T> &norm, const Vector<T> &point)
{
  auto normalized = norm.normalized();
  return Plane{normalized, normalized.dot(point)};
}

template <std::floating_point T>
Plane<T> Plane<T>::getNormalDist(const Vector<T> &norm, T dist)
{
  auto normalized = norm.normalized();
  return Plane{normalized, dist};
}

template <std::floating_point T>
bool operator==(const Plane<T> &pl, const Plane<T> &pr)
{
  return pl.isEqual(pr);
}

template <std::floating_point T>
std::ostream &operator<<(std::ostream &ost, const Plane<T> &p)
{
  ost << p.norm() << " * X = " << p.dist();
  return ost;
}

} // namespace geom

#endif // __INCLUDE_PRIMITIVES_PLANE_HH__
