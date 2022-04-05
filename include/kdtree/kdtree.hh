#ifndef __INCLUDE_KDTREE_KDTREE_HH__
#define __INCLUDE_KDTREE_KDTREE_HH__

#include <cassert>
#include <initializer_list>
#include <memory>
#include <vector>

#include "primitives/primitives.hh"

#include "container.hh"
#include "node.hh"

namespace geom::kdtree
{

template <std::floating_point T>
class KdTree
{
private:
  std::unique_ptr<Node<T>> root_;
  std::vector<Triangle<T>> triangles_;

public:
  KdTree();
  KdTree(std::initializer_list<Triangle<T>> il);
  ~KdTree();

  class ConstIterator;

  // ConstIterators
  ConstIterator cbegin() const;
  ConstIterator cend() const;

  // Modifiers
  ConstIterator insert(const Triangle<T> &tr);
  void clear();

  // Capacity
  bool empty() const;
  size_t size() const;

  class ConstIterator final
  {
  public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::size_t;
    using value_type = Container<T>;
    using reference = Container<T>;
    using pointer = std::unique_ptr<Container<T>>;

  private:
    KdTree<T> *tree_;
    Node<T> *node_;

  public:
    ConstIterator(const KdTree<T> *tree, const Node<T> *node);
    ConstIterator(const ConstIterator &iter) = default;
    ConstIterator(ConstIterator &&iter) = default;

    ConstIterator &operator=(const ConstIterator &cont) = default;
    ConstIterator &operator=(ConstIterator &&cont) = default;

    ~ConstIterator() = default;

    ConstIterator operator++();
    ConstIterator operator++(int);

    reference operator*() const;
    pointer operator->() const;

    bool operator==(const ConstIterator &lhs) const;
    bool operator!=(const ConstIterator &lhs) const;
  };
};

//============================================================================================
//                                    KdTree definitions
//============================================================================================

//============================================================================================
//                             KdTree::ConstIterator definitions
//============================================================================================

template <std::floating_point T>
KdTree<T>::ConstIterator::ConstIterator(const KdTree<T> *tree, const Node<T> *node)
  : tree_(tree), node_(node)
{}

template <std::floating_point T>
typename KdTree<T>::ConstIterator KdTree<T>::ConstIterator::operator++()
{
  assert(false && "Not implemented yet");
}

template <std::floating_point T>
typename KdTree<T>::ConstIterator KdTree<T>::ConstIterator::operator++(int)
{
  assert(false && "Not implemented yet");
}

template <std::floating_point T>
typename KdTree<T>::ConstIterator::reference KdTree<T>::ConstIterator::operator*() const
{
  return Container<T>{tree_, node_};
}

template <std::floating_point T>
typename KdTree<T>::ConstIterator::pointer KdTree<T>::ConstIterator::operator->() const
{
  return std::make_unique(tree_, node_);
}

template <std::floating_point T>
bool KdTree<T>::ConstIterator::operator==(const KdTree<T>::ConstIterator &lhs) const
{
  return (tree_ == lhs.tree_) && (node_ == lhs.node_);
}

template <std::floating_point T>
bool KdTree<T>::ConstIterator::operator!=(const KdTree<T>::ConstIterator &lhs) const
{
  return !operator==(lhs);
}

} // namespace geom::kdtree

#endif // __INCLUDE_KDTREE_KDTREE_HH__
