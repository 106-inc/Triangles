#ifndef __INCLUDE_PRIMITIVES_LINE_HH__
#define __INCLUDE_PRIMITIVES_LINE_HH__

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
  bool isEqual(const Line &line) const;

  static Line getBy2Points(const Vector<T> &p1, const Vector<T> &p2);
};

template <std::floating_point T>
std::ostream &operator<<(std::ostream &ost, const Line<T> &line)
{
  ost << line.org() << " + " << line.dir() << " * t";
  return ost;
}

template <std::floating_point T>
bool operator==(const Line<T> &lhs, const Line<T> &rhs)
{
  return lhs.isEqual(rhs);
}

template <std::floating_point T>
Line<T>::Line(const Vector<T> &org, const Vector<T> &dir) : org_{org}, dir_{dir}
{
  if (dir_ == Vector<T>{0})
    throw std::logic_error{"Direction vector equals zero."};
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
bool Line<T>::isEqual(const Line<T> &line) const
{
  return belongs(line.org_) && dir_.isPar(line.dir_);
}

template <std::floating_point T>
Line<T> Line<T>::getBy2Points(const Vector<T> &p1, const Vector<T> &p2)
{
  return Line<T>{p1, p2 - p1};
}

} // namespace geom

#endif // __INCLUDE_PRIMITIVES_LINE_HH__
