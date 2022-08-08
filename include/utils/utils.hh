#ifndef __INCLUDE_UTILS_UTILS_HH__
#define __INCLUDE_UTILS_UTILS_HH__

#include <cassert>
#include <concepts>
#include <iterator>

namespace utils
{
template <class T>
concept ItContainer = requires(T cont)
{
  {
    std::begin(cont)
    } -> std::forward_iterator;
  {
    std::end(cont)
    } -> std::forward_iterator;
  {
    std::size(cont)
    } -> std::convertible_to<std::size_t>;
};

namespace detail
{
template <std::forward_iterator It>
class EnumerateIt final
{
private:
  template <typename Ref>
  struct ArrowProxy
  {
    Ref r;

    Ref *operator->()
    {
      return &r;
    }
  };

public:
  using iterator_category = std::forward_iterator_tag;
  using difference_type =
    std::pair<std::make_signed_t<std::size_t>, typename std::iterator_traits<It>::difference_type>;
  using value_type = std::pair<std::size_t, typename std::iterator_traits<It>::value_type>;
  using reference = std::pair<std::size_t, typename std::iterator_traits<It>::reference>;
  using pointer = ArrowProxy<reference>;

private:
  std::size_t counter_;
  It iter_;

public:
  EnumerateIt(std::size_t i, It iter) : counter_(i), iter_(iter)
  {}

  reference operator*() const
  {
    return {counter_, *iter_};
  }

  EnumerateIt &operator++()
  {
    ++counter_;
    ++iter_;
    return *this;
  }

  EnumerateIt operator++(int)
  {
    auto temp{*this};
    operator++();
    return temp;
  }

  bool equals(const EnumerateIt &rhs) const
  {
    return iter_ == rhs.iter_;
  }

  pointer operator->() const
  {
    return pointer{{counter_, *iter_}};
  }
};

template <std::forward_iterator It>
bool operator==(const EnumerateIt<It> &lhs, const EnumerateIt<It> &rhs)
{
  return lhs.equals(rhs);
}
} // namespace detail

template <ItContainer C>
class Enumerate final
{
private:
  using NoRefC = std::remove_reference_t<C>;
  using ContStorageType = std::conditional_t<std::is_rvalue_reference_v<C>, NoRefC, NoRefC &>;

  ContStorageType cont_;
  using EnumItType = detail::EnumerateIt<decltype(std::begin(cont_))>;

public:
  Enumerate(C &&cont) : cont_(std::forward<C>(cont))
  {}

  auto begin()
  {
    return EnumItType(0, std::begin(cont_));
  }
  auto begin() const
  {
    return EnumItType(0, std::begin(cont_));
  }

  auto end()
  {
    return EnumItType(std::size(cont_), std::end(cont_));
  }
  auto end() const
  {
    return EnumItType(std::size(cont_), std::end(cont_));
  }

  auto size() const
  {
    return std::size(cont_);
  }
};

template <ItContainer C>
Enumerate(C &&) -> Enumerate<C &&>;

} // namespace utils

#endif /* __INCLUDE_UTILS_UTILS_HH__ */
