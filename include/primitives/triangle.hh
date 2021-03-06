#ifndef __INCLUDE_PRIMITIVES_TRIANGLE_HH__
#define __INCLUDE_PRIMITIVES_TRIANGLE_HH__

#include <algorithm>
#include <array>

#include "boundbox.hh"
#include "plane.hh"
#include "vec3.hh"

/**
 * @brief triangle.hh
 * Triangle class implementation
 */

namespace geom
{

/**
 * @class Triangle
 * @brief Triangle class implementation
 *
 * @tparam T - floating point type of coordinates
 */
template <std::floating_point T>
class Triangle final
{
private:
  /**
   * @brief Vertices of triangle
   */
  std::array<Vec3<T>, 3> vertices_;

public:
  using Iterator = typename std::array<Vec3<T>, 3>::iterator;
  using ConstIterator = typename std::array<Vec3<T>, 3>::const_iterator;

  /**
   * @brief Construct a new Triangle object
   */
  Triangle();

  /**
   * @brief Construct a new Triangle object from 3 points
   *
   * @param[in] p1 1st point
   * @param[in] p2 2nd point
   * @param[in] p3 3rd point
   */
  Triangle(const Vec3<T> &p1, const Vec3<T> &p2, const Vec3<T> &p3);

  /**
   * @brief Overloaded operator[] to get access to vertices
   *
   * @param[in] idx index of vertex
   * @return const Vec3<T>& const reference to vertex
   */
  const Vec3<T> &operator[](std::size_t idx) const &;

  /**
   * @brief Overloaded operator[] to get access to vertices
   *
   * @param[in] idx index of vertex
   * @return Vec3<T>&& reference to vertex
   */
  Vec3<T> &&operator[](std::size_t idx) &&;

  /**
   * @brief Overloaded operator[] to get access to vertices
   *
   * @param[in] idx index of vertex
   * @return Vec3<T>& reference to vertex
   */
  Vec3<T> &operator[](std::size_t idx) &;

  /**
   * @brief Get begin iterator
   * @return Iterator
   */
  Iterator begin() &;

  /**
   * @brief Get end iterator
   * @return Iterator
   */
  Iterator end() &;

  /**
   * @brief Get begin const iterator
   * @return ConstIterator
   */
  ConstIterator begin() const &;

  /**
   * @brief Get end const iterator
   * @return ConstIterator
   */
  ConstIterator end() const &;

  /**
   * @brief Get triangle's plane
   *
   * @return Plane<T>
   */
  Plane<T> getPlane() const;

  /**
   * @brief Check is triangle valid
   *
   * @return true if triangle is valid
   * @return false if triangle is invalid
   */
  bool isValid() const;

  /**
   * @brief Returns triangle's bound box
   *
   * @return BoundBox<T>
   */
  BoundBox<T> boundBox() const;

  /**
   * @brief Checks if this Triangle belongs to BoundBox
   *
   * @param[in] bb BoundBox
   * @return true if Triangle belongs to BoundBox
   * @return false if Triangle doesn't belong to BoundBox
   */
  bool belongsTo(const BoundBox<T> &bb) const;
};

/**
 * @brief Triangle print operator
 *
 * @tparam T - floating point type of coordinates
 * @param[in, out] ost output stream
 * @param[in] tr Triangle to print
 * @return std::ostream& modified ostream instance
 */
template <std::floating_point T>
std::ostream &operator<<(std::ostream &ost, const Triangle<T> &tr)
{
  ost << "Triangle: {";
  for (std::size_t i = 0; i < 3; ++i)
    ost << tr[i] << (i == 2 ? "" : ", ");

  ost << "}";

  return ost;
}

template <std::floating_point T>
std::istream &operator>>(std::istream &ist, Triangle<T> &tr)
{
  ist >> tr[0] >> tr[1] >> tr[2];
  return ist;
}

template <std::floating_point T>
Triangle<T>::Triangle() : vertices_()
{}

template <std::floating_point T>
Triangle<T>::Triangle(const Vec3<T> &p1, const Vec3<T> &p2, const Vec3<T> &p3)
  : vertices_{p1, p2, p3}
{}

template <std::floating_point T>
const Vec3<T> &Triangle<T>::operator[](std::size_t idx) const &
{
  return vertices_[idx % 3];
}

template <std::floating_point T>
Vec3<T> &&Triangle<T>::operator[](std::size_t idx) &&
{
  return std::move(vertices_[idx % 3]);
}

template <std::floating_point T>
Vec3<T> &Triangle<T>::operator[](std::size_t idx) &
{
  return vertices_[idx % 3];
}

template <std::floating_point T>
typename Triangle<T>::Iterator Triangle<T>::begin() &
{
  return vertices_.begin();
}

template <std::floating_point T>
typename Triangle<T>::Iterator Triangle<T>::end() &
{
  return vertices_.end();
}

template <std::floating_point T>
typename Triangle<T>::ConstIterator Triangle<T>::begin() const &
{
  return vertices_.begin();
}

template <std::floating_point T>
typename Triangle<T>::ConstIterator Triangle<T>::end() const &
{
  return vertices_.end();
}

template <std::floating_point T>
Plane<T> Triangle<T>::getPlane() const
{
  return Plane<T>::getBy3Points(vertices_[0], vertices_[1], vertices_[2]);
}

template <std::floating_point T>
bool Triangle<T>::isValid() const
{
  auto edge1 = vertices_[1] - vertices_[0];
  auto edge2 = vertices_[2] - vertices_[0];

  auto cross12 = cross(edge1, edge2);
  return (cross12 != Vec3<T>{});
}

template <std::floating_point T>
BoundBox<T> Triangle<T>::boundBox() const
{
  auto minMaxX = std::minmax({vertices_[0].x, vertices_[1].x, vertices_[2].x});
  auto minMaxY = std::minmax({vertices_[0].y, vertices_[1].y, vertices_[2].y});
  auto minMaxZ = std::minmax({vertices_[0].z, vertices_[1].z, vertices_[2].z});

  return {
    minMaxX.first - ThresComp<T>::getThreshold(), minMaxX.second + ThresComp<T>::getThreshold(),
    minMaxY.first - ThresComp<T>::getThreshold(), minMaxY.second + ThresComp<T>::getThreshold(),
    minMaxZ.first - ThresComp<T>::getThreshold(), minMaxZ.second + ThresComp<T>::getThreshold()};
}

template <std::floating_point T>
bool Triangle<T>::belongsTo(const BoundBox<T> &bb) const
{
  return boundBox().belongsTo(bb);
}

} // namespace geom

#endif // __INCLUDE_PRIMITIVES_TRIANGLE_HH__
