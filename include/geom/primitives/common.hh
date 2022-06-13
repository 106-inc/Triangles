#ifndef __INCLUDE_PRIMITIVES_COMMON_HH__
#define __INCLUDE_PRIMITIVES_COMMON_HH__

#include <cmath>
#include <concepts>
#include <cstdint>
#include <limits>
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

enum class Axis : std::int8_t
{
  X = 0,
  Y = 1,
  Z = 2,
  NONE
};

template <Number T>
class ThresComp final
{
private:
  static inline T threshold_ = 1e2 * std::numeric_limits<T>::epsilon();

public:
  ThresComp() = delete;

  static void setThreshold(T thres) requires std::is_floating_point_v<T>
  {
    threshold_ = thres;
  }

  static T getThreshold() requires std::is_floating_point_v<T>
  {
    return threshold_;
  }

  static void scaleThreshold(T factor) requires std::is_floating_point_v<T>
  {
    threshold_ *= factor;
  }

  static void resetThreshold() requires std::is_floating_point_v<T>
  {
    threshold_ = std::numeric_limits<T>::epsilon();
  }

  static bool isEqual(T lhs, T rhs)
  {
    if constexpr (std::is_floating_point_v<T>)
      return std::abs(rhs - lhs) < threshold_;
    else
      return lhs == rhs;
  }

  static bool isZero(T num)
  {
    return isEqual(num, T{});
  }
};

template <Number T>
bool isEqualThreshold(T num1, T num2)
{
  return ThresComp<T>::isEqual(num1, num2);
}

template <Number T>
bool isZeroThreshold(T num)
{
  return ThresComp<T>::isZero(num);
}

} // namespace geom

#endif // __INCLUDE_PRIMITIVES_COMMON_HH__
