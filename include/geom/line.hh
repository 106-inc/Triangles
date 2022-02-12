#ifndef __INCLUDE_GEOM_LINE_HH__
#define __INCLUDE_GEOM_LINE_HH__

#include "vector.hh"

namespace geom
{

template <std::floating_point T>
class Line final
{
private:
  Vector<T> org_{}; // origin
  Vector<T> dir_{}; // direction

public:
  Line(const Vector<T> &org, const Vector<T> &dir);

  const Vector<T> &org() const;
  const Vector<T> &dir() const;

  bool belongs(const Vector<T> &point) const;

  static Line getBy2P(const Vector<T> &p1, const Vector<T> &p2);
};

template <std::floating_point T>
Line<T>::Line(const Vector<T> &org, const Vector<T> &dir) : org_{org}, dir_{dir}
{
}

template <std::floating_point T>
const Vector<T> &Line<T>::org() const
{
  return org_;
}

template <std::floating_point T>
const Vector<T> &Line<T>::dir() const
{
  return dir_;
}

template <std::floating_point T>
bool Line<T>::belongs(const Vector<T> &point) const
{
  return dir_.cross(point - org_) == Vector<T>{0};
}

template <std::floating_point T>
Line<T> Line<T>::getBy2P(const Vector<T> &p1, const Vector<T> &p2)
{
  return Line<T>{p2 - p1, p1};
}

template <std::floating_point T>
std::ostream &operator<<(std::ostream &ost, const Line<T> &line)
{
  ost << line.org() << " + " << line.dir() << " * t";
  return ost;
}

// TODO(Tako): define operator==

} // namespace geom

#endif // __INCLUDE_GEOM_LINE_HH__
