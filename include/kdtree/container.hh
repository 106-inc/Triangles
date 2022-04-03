#ifndef __INCLUDE_KDTREE_CONTAINER_HH__
#define __INCLUDE_KDTREE_CONTAINER_HH__

#include <concepts>
#include <exception>

#include "node.hh"

namespace geom::kdtree
{

template <std::floating_point T>
class KdTree;

template <std::floating_point T>
class Container final
{
private:
  KdTree<T> *tree_;
  Node<T> *node_;

public:
  Container(const KdTree<T> *tree, const Node<T> *node);
  Container(const Container &cont) = default;
  Container(Container &&cont) = default;
  ~Container() = default;

  Container &operator=(const Container &cont) = default;
  Container &operator=(Container &&cont) = default;

  class ConstIterator;
  ConstIterator cbegin() const;
  ConstIterator cend() const;

  BoundBox<T> boundBox() const;

  class ConstIterator final
  {
  public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::size_t;
    using value_type = Triangle<T>;
    using reference = Triangle<T> &;
    using pointer = Triangle<T> *;

  private:
    Container *cont_;
    std::vector<Index>::iterator curIdxIt_;

  public:
    ConstIterator(const Container *cont, bool isEnd = false);
    ConstIterator(const ConstIterator &iter) = default;
    ConstIterator(ConstIterator &&iter) = default;

    ConstIterator &operator=(const ConstIterator &cont) = default;
    ConstIterator &operator=(ConstIterator &&cont) = default;

    ~ConstIterator() = default;

    ConstIterator operator++();
    ConstIterator operator++(int);

    reference operator*() const;
    pointer operator->() const;

    bool operator==(const ConstIterator &lhs);
    bool operator!=(const ConstIterator &lhs);
  };
};

//============================================================================================
//                                    Container definitions
//============================================================================================

template <std::floating_point T>
typename Container<T>::ConstIterator Container<T>::cbegin() const
{
  return ConstIterator(this);
}

template <std::floating_point T>
typename Container<T>::ConstIterator Container<T>::cend() const
{
  return ConstIterator(this, /* isEnd = */ true);
}

template <std::floating_point T>
BoundBox<T> Container<T>::boundBox() const
{
  return node_->boundBox_;
}

//============================================================================================
//                                Container::ConstIterator definitions
//============================================================================================

template <std::floating_point T>
Container<T>::ConstIterator::ConstIterator(const Container<T> *cont, bool isEnd) : cont_(cont)
{
  if (nullptr == cont_)
    throw std::invalid_argument("Tried to create iterator with invalid Container pointer");

  if (isEnd)
    curIdxIt_ = cont->end();
  else
    curIdxIt_ = cont_->begin();
}

template <std::floating_point T>
typename Container<T>::ConstIterator Container<T>::ConstIterator::operator++()
{
  return ++curIdxIt_;
}

template <std::floating_point T>
typename Container<T>::ConstIterator Container<T>::ConstIterator::operator++(int)
{
  return curIdxIt_++;
}

template <std::floating_point T>
typename Container<T>::ConstIterator::reference Container<T>::ConstIterator::operator*() const
{
  return cont_->tree_->triangles_[*curIdxIt_];
}

template <std::floating_point T>
typename Container<T>::ConstIterator::pointer Container<T>::ConstIterator::operator->() const
{
  return &cont_->tree_->triangles_[*curIdxIt_];
}

template <std::floating_point T>
bool Container<T>::ConstIterator::operator==(const typename Container<T>::ConstIterator &lhs)
{
  return (cont_ == lhs.cont_) && (curIdxIt_ == lhs.curIdxIt_);
}

template <std::floating_point T>
bool Container<T>::ConstIterator::operator!=(const typename Container<T>::ConstIterator &lhs)
{
  return !(operator==(lhs));
}

} // namespace geom::kdtree

#endif // __INCLUDE_KDTREE_CONTAINER_HH__
