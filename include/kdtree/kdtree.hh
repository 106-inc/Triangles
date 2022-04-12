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
  KdTree(std::initializer_list<Triangle<T>> il);
  KdTree(const KdTree &tree);
  KdTree(KdTree &&tree) = default;
  KdTree() = default;
  ~KdTree();

  KdTree &operator=(const KdTree &tree);
  KdTree &operator=(KdTree &&tree) = default;

  class ConstIterator;

  // ConstIterators
  ConstIterator cbegin() const;
  ConstIterator cend() const;

  // Modifiers
  void insert(const Triangle<T> &tr);
  void clear();

  // Capacity
  bool empty() const;
  size_t size() const;

private:
  void expandingInsert(const Triangle<T> &tr);
  void tryExpandRight(Axis axis, const BoundBox<T> &trianBB);
  void tryExpandLeft(Axis axis, const BoundBox<T> &trianBB);

  void nonExpandingInsert(const Triangle<T> &tr);

public:
  class ConstIterator final
  {
  public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::size_t;
    using value_type = Container<T>;
    using reference = Container<T>;
    using pointer = std::unique_ptr<Container<T>>;

  private:
    const KdTree<T> *tree_;
    const Node<T> *node_;

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

template <std::floating_point T>
KdTree<T>::KdTree(std::initializer_list<Triangle<T>> il)
{
  for (const auto &tr : il)
    insert(tr);
}

template <std::floating_point T>
KdTree<T>::KdTree(const KdTree<T> &tree)
{
  // temporary solution
  for (const auto &tr : tree.triangles_)
    insert(tr);
}

template <std::floating_point T>
KdTree<T>::~KdTree()
{
  clear();
}

template <std::floating_point T>
KdTree<T> &KdTree<T>::operator=(const KdTree<T> &tree)
{
  KdTree tmp{tree};
  operator=(std::move(tmp));
  return *this;
}

// ConstIterators
template <std::floating_point T>
typename KdTree<T>::ConstIterator KdTree<T>::cbegin() const
{
  assert(false && "Not implemented yet");
}

template <std::floating_point T>
typename KdTree<T>::ConstIterator KdTree<T>::cend() const
{
  assert(false && "Not implemented yet");
}

// Modifiers
template <std::floating_point T>
void KdTree<T>::insert(const Triangle<T> &tr)
{
  if (nullptr == root_)
  {
    root_ = std::unique_ptr<Node<T>>{new Node<T>{T{}, Axis::NONE, tr.boundBox(), {0}}};
    triangles_.push_back(tr);
    return;
  }

  if (tr.belongsTo(root_->boundBox))
    nonExpandingInsert(tr);
  else
    expandingInsert(tr);
}

template <std::floating_point T>
void KdTree<T>::clear()
{
  assert(false && "Not implemented yet");
}

// Capacity
template <std::floating_point T>
bool KdTree<T>::empty() const
{
  return triangles_.empty();
}

template <std::floating_point T>
size_t KdTree<T>::size() const
{
  return triangles_.size();
}

template <std::floating_point T>
void KdTree<T>::expandingInsert(const Triangle<T> &tr)
{
  auto trianBB = tr.boundBox();
  auto index = triangles_.size();
  triangles_.push_back(tr);

  for (auto axis : {Axis::X, Axis::Y, Axis::Z})
    tryExpandRight(axis, trianBB);

  for (auto axis : {Axis::X, Axis::Y, Axis::Z})
    tryExpandLeft(axis, trianBB);

  root_->indicies.push_back(index);
}

template <std::floating_point T>
void KdTree<T>::tryExpandRight(Axis axis, const BoundBox<T> &trianBB)
{
  const auto &rootBB = root_->boundBox;
  if (trianBB.max(axis) <= rootBB.max(axis))
    return;

  BoundBox<T> newRightBB = rootBB;
  newRightBB.min(axis) = rootBB.max(axis);
  newRightBB.max(axis) = trianBB.max(axis);

  BoundBox<T> newRootBB = rootBB;
  newRootBB.max(axis) = rootBB.max(axis);

  std::unique_ptr<Node<T>> newRight{new Node<T>{T{}, Axis::NONE, newRightBB}};
  std::unique_ptr<Node<T>> newRoot{new Node<T>{rootBB.max(axis), axis, newRootBB}};

  newRoot->right = std::move(newRight);
  newRoot->left = std::move(root_);

  root_ = std::move(newRoot);
}

template <std::floating_point T>
void KdTree<T>::tryExpandLeft(Axis axis, const BoundBox<T> &trianBB)
{
  const auto &rootBB = root_->boundBox;
  if (trianBB.min(axis) >= rootBB.min(axis))
    return;

  BoundBox<T> newLeftBB = rootBB;
  newLeftBB.max(axis) = rootBB.min(axis);
  newLeftBB.min(axis) = trianBB.min(axis);

  BoundBox<T> newRootBB = rootBB;
  newRootBB.min(axis) = rootBB.min(axis);

  std::unique_ptr<Node<T>> newLeft{new Node<T>{T{}, Axis::NONE, newLeftBB}};
  std::unique_ptr<Node<T>> newRoot{new Node<T>{rootBB.min(axis), axis, newRootBB}};

  newRoot->left = std::move(newLeft);
  newRoot->right = std::move(root_);

  root_ = std::move(newRoot);
}

template <std::floating_point T>
void KdTree<T>::nonExpandingInsert(const Triangle<T> &tr)
{}

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
