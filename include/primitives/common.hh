#ifndef __INCLUDE_PRIMITIVES_COMMON_HH__
#define __INCLUDE_PRIMITIVES_COMMON_HH__

#include <concepts>

namespace geom
{
/**
 * @concept Number
 * @brief Useful concept which represents floating point and integral types
 *
 * @tparam T
 */
template <class T>
concept Number = std::is_floating_point_v<T> || std::is_integral_v<T>;

} // namespace geom

#endif // __INCLUDE_PRIMITIVES_COMMON_HH__