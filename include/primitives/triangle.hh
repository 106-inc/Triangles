#ifndef __INCLUDE_PRIMITIVES_TRIANGLE_HH__
#define __INCLUDE_PRIMITIVES_TRIANGLE_HH__

#include <array>
#include <concepts>

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
  const Vec3<T> &operator[](std::size_t idx) const;

  /**
   * @brief Overloaded operator[] to get access to vertices
   *
   * @param[in] idx index of vertex
   * @return Vec3<T>& reference to vertex
   */
  Vec3<T> &operator[](std::size_t idx);

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
  for (size_t i = 0; i < 3; ++i)
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
const Vec3<T> &Triangle<T>::operator[](std::size_t idx) const
{
  return vertices_[idx % 3];
}

template <std::floating_point T>
Vec3<T> &Triangle<T>::operator[](std::size_t idx)
{
  return vertices_[idx % 3];
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

} // namespace geom

#endif // __INCLUDE_PRIMITIVES_TRIANGLE_HH__
