#ifndef __INCLUDE_KDTREE_CONTAINER_HH__
#define __INCLUDE_KDTREE_CONTAINER_HH__

#include <concept>
#include <exception>
#include <iterator>

#include "node.hh"

namespace geom::kdtree
{

enum class IterType
{
  BEGIN,
  END
};

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
  Container(const Container &cont);
  Container(Container &&cont);
  ~Container() = default;

  Container &operator=(const Container &cont);
  Container &operator=(Container &&cont);

  Iterator begin() const;
  Iterator end() const;

  BoundBox boundBox() const;

  class Iterator final
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
    Iterator(const Container *cont, IterType it = IterType::BEGIN);
    Iterator(const Iterator &iter);
    Iterator(Iterator &&iter);

    Iterator &operator=(const Iterator &cont);
    Iterator &operator=(Iterator &&cont);

    ~Iterator() = default;

    void operator++();
    Iterator operator++(int);

    reference operator*() const;
    pointer operator->() const;

    bool operator==(const Iterator &lhs);
    bool operator!=(const Iterator &lhs);
  };
};

//============================================================================================
//                                    Container definitions
//============================================================================================

template <std::floating_point T>
Container<T>::Container(const KdTree<T> *tree, const Node<T> *node) : tree_(tree), node_(node)
{}

template <std::floating_point T>
Container<T>::Container(const Container &cont) : tree_(cont.tree_), node_(cont.node_)
{}

template <std::floating_point T>
Container<T>::Container(Container &&cont) : tree_(cont.tree_), node_(cont.node_)
{}

template <std::floating_point T>
Container<T> &Container<T>::operator=(const Container &cont)
{
  node_ = cont.node_;
  tree_ = cont.tree_;
  return *this;
}

template <std::floating_point T>
Container<T> &Container<T>::operator=(Container &&cont)
{
  node_ = cont.node_;
  tree_ = cont.tree_;
  return *this;
}

template <std::floating_point T>
Container<T>::Iterator Container<T>::begin() const
{
  return Iterator(this);
}

template <std::floating_point T>
Container<T>::Iterator Container<T>::end() const
{
  return Iterator(this, IterType::END);
}

template <std::floating_point T>
BoundBox Container<T>::boundBox() const
{
  return node_->boundBox_;
}

//============================================================================================
//                                Container::Iterator definitions
//============================================================================================

template <std::floating_point T>
Containter<T>::Iterator::Iterator(const Container<T> *cont, IterType it) : cont_(cont)
{
  if (nullptr == cont_)
    throw std::invalid_argument("Tried to create iterator with invalid Container pointer");

  switch (it)
  {
  case IterType::BEGIN:
    curIdxIt_ = cont_->begin();
    break;
  case IterType::END:
    curIdxIt_ = cont->end();
    break;
  default:
    throw std::invalid_argument("Tried to create iterator with invalid Container pointer");
    break;
  }
}

template <std::floating_point T>
Containter<T>::Iterator::Iterator(const Container<T>::Iterator &iter)
  : cont_(iter.cont_), curIdxIt_(iter.curIdxIt_)
{}

template <std::floating_point T>
Containter<T>::Iterator::Iterator(Container<T>::Iterator &&iter)
  : cont_(iter.cont_), curIdxIt_(iter.curIdxIt_)
{}

template <std::floating_point T>
Containter<T>::Iterator::Iterator &operator=(const Container<T>::Iterator &iter)
{
  cont_ = iter.cont_;
  curIdxIt_ = iter.curIdxIt_;
}

template <std::floating_point T>
Containter<T>::Iterator::Iterator &operator=(Container<T>::Iterator &&iter)
{
  cont_ = iter.cont_;
  curIdxIt_ = iter.curIdxIt_;
}

template <std::floating_point T>
void Containter<T>::Iterator::operator++()
{
  ++curIdxIt_;
}

template <std::floating_point T>
Containter<T>::Iterator Containter<T>::Iterator::operator++(int)
{
  return curIdxIt_++;
}

template <std::floating_point T>
Containter<T>::Iterator::reference Containter<T>::Iterator::operator*() const
{
  return cont_->tree_->triangles_[*curIdxIt_];
}

template <std::floating_point T>
Containter<T>::Iterator::pointer Containter<T>::Iterator::operator->() const
{
  return &cont_->tree_->triangles_[*curIdxIt_];
}

template <std::floating_point T>
bool Containter<T>::Iterator::operator==(const Container<T>::Iterator &lhs)
{
  return (cont_ == lhs.cont_) && (curIdxIt_ == lhs.curIdxIt_);
}

template <std::floating_point T>
bool Containter<T>::Iterator::operator!=(const Container<T>::Iterator &lhs)
{
  return !(operator==(lhs));
}

} // namespace geom::kdtree

#endif // __INCLUDE_KDTREE_CONTAINER_HH__
