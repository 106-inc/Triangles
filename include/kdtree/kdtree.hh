#ifndef __INCLUDE_KDTREE_KDTREE_HH__
#define __INCLUDE_KDTREE_KDTREE_HH__

#include <cassert>
#include <functional>
#include <initializer_list>
#include <memory>
#include <queue>
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
  std::size_t nodeCapacity_{1};

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

  ConstIterator begin() const;
  ConstIterator end() const;

  ConstIterator beginFrom(const ConstIterator &iter) const;

  // Modifiers
  void insert(const Triangle<T> &tr);
  void clear();
  void setNodeCapacity(std::size_t newCap);

  // Capacity
  bool empty() const;
  std::size_t size() const;
  std::size_t nodeCapacity() const;

  const Triangle<T> &triangleByIndex(Index index) const;

  void dumpRecursive(std::ostream &ost = std::cout) const;

  static bool isOnPosSide(Axis axis, T separator, const Triangle<T> &tr);
  static bool isOnNegSide(Axis axis, T separator, const Triangle<T> &tr);
  static bool isOnSide(Axis axis, T separator, const Triangle<T> &tr,
                       std::function<bool(T, T)> comparator);

private:
  void expandingInsert(const Triangle<T> &tr);
  void tryExpandRight(Axis axis, const BoundBox<T> &trianBB);
  void tryExpandLeft(Axis axis, const BoundBox<T> &trianBB);

  void nonExpandingInsert(Node<T> *node, const Triangle<T> &tr, Index index, bool isSubdiv = false);
  bool isDivisable(const Node<T> *node);
  void subdivide(Node<T> *node);

public:
  class ConstIterator final
  {
  public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::size_t;
    using value_type = Container<T>;
    using reference = Container<T>;
    using pointer = void;
    // using pointer = std::unique_ptr<Container<T>>;

  private:
    const KdTree<T> *tree_;
    const Node<T> *node_;
    std::queue<const Node<T> *> fifo_;

  public:
    ConstIterator(const KdTree<T> *tree, const Node<T> *node);
    ConstIterator(const ConstIterator &iter) = default;
    ConstIterator(ConstIterator &&iter) = default;

    ConstIterator &operator=(const ConstIterator &cont) = default;
    ConstIterator &operator=(ConstIterator &&cont) = default;

    ~ConstIterator() = default;

    ConstIterator &operator++();
    ConstIterator operator++(int);

    reference operator*() const;
    // pointer operator->() const;

    bool operator==(const ConstIterator &lhs) const;
    bool operator!=(const ConstIterator &lhs) const;

    static ConstIterator beginFrom(const ConstIterator &iter);
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
  return ConstIterator{this, root_.get()};
}

template <std::floating_point T>
typename KdTree<T>::ConstIterator KdTree<T>::cend() const
{
  return ConstIterator{this, nullptr};
}

template <std::floating_point T>
typename KdTree<T>::ConstIterator KdTree<T>::begin() const
{
  return cbegin();
}

template <std::floating_point T>
typename KdTree<T>::ConstIterator KdTree<T>::end() const
{
  return cend();
}

template <std::floating_point T>
typename KdTree<T>::ConstIterator KdTree<T>::beginFrom(
  const typename KdTree<T>::ConstIterator &iter) const
{
  return KdTree<T>::ConstIterator::beginFrom(iter);
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

  if (!tr.belongsTo(root_->boundBox))
    expandingInsert(tr);
  else
  {
    auto index = triangles_.size();
    triangles_.push_back(tr);
    nonExpandingInsert(root_.get(), tr, index);
  }
}

template <std::floating_point T>
void KdTree<T>::clear()
{
  // Temporary recursive solution
  root_.reset();
}

template <std::floating_point T>
void KdTree<T>::setNodeCapacity(std::size_t newCap)
{
  nodeCapacity_ = newCap;
}

// Capacity
template <std::floating_point T>
bool KdTree<T>::empty() const
{
  return triangles_.empty();
}

template <std::floating_point T>
std::size_t KdTree<T>::size() const
{
  return triangles_.size();
}

template <std::floating_point T>
std::size_t KdTree<T>::nodeCapacity() const
{
  return nodeCapacity_;
}

template <std::floating_point T>
const Triangle<T> &KdTree<T>::triangleByIndex(Index index) const
{
  return triangles_[index];
}

template <std::floating_point T>
void KdTree<T>::dumpRecursive(std::ostream &ost) const
{
  ost << "digraph kdtree {" << std::endl;
  if (root_)
    root_->dumpRecursive(ost);
  ost << "}" << std::endl;
}

template <std::floating_point T>
bool KdTree<T>::isOnPosSide(Axis axis, T separator, const Triangle<T> &tr)
{
  return isOnSide(axis, separator, tr, std::greater<T>{});
}

template <std::floating_point T>
bool KdTree<T>::isOnNegSide(Axis axis, T separator, const Triangle<T> &tr)
{
  return isOnSide(axis, separator, tr, std::less<T>{});
}

template <std::floating_point T>
bool KdTree<T>::isOnSide(Axis axis, T separator, const Triangle<T> &tr,
                         std::function<bool(T, T)> comparator)
{
  if (Axis::NONE == axis)
    return false;

  auto axisIdx = static_cast<size_t>(axis);
  for (size_t i = 0; i < 3; ++i)
    if (!comparator(tr[i][axisIdx], separator))
      return false;

  return true;
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

  auto newRightBB = rootBB;
  newRightBB.min(axis) = rootBB.max(axis);
  newRightBB.max(axis) = trianBB.max(axis);

  auto newRootBB = rootBB;
  newRootBB.max(axis) = newRightBB.max(axis);

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
  newRootBB.min(axis) = newLeftBB.min(axis);

  std::unique_ptr<Node<T>> newLeft{new Node<T>{T{}, Axis::NONE, newLeftBB}};
  std::unique_ptr<Node<T>> newRoot{new Node<T>{rootBB.min(axis), axis, newRootBB}};

  newRoot->left = std::move(newLeft);
  newRoot->right = std::move(root_);

  root_ = std::move(newRoot);
}

template <std::floating_point T>
void KdTree<T>::nonExpandingInsert(Node<T> *node, const Triangle<T> &tr, Index index, bool isSubdiv)
{
  auto curNode = node;
  while (true)
  {
    if (isOnPosSide(curNode->sepAxis, curNode->separator, tr))
      curNode = curNode->right.get();
    else if (isOnNegSide(curNode->sepAxis, curNode->separator, tr))
      curNode = curNode->left.get();
    else
      break;
  }

  curNode->indicies.push_back(index);
  if (isDivisable(curNode) && (!isSubdiv))
    subdivide(curNode);
}

template <std::floating_point T>
bool KdTree<T>::isDivisable(const Node<T> *node)
{
  return (node->indicies.size() > nodeCapacity_) && (node->sepAxis == Axis::NONE);
}

template <std::floating_point T>
void KdTree<T>::subdivide(Node<T> *node)
{
  const auto &nodeBB = node->boundBox;
  auto axis = node->sepAxis = nodeBB.getMaxDim();
  auto sep = node->separator = nodeBB.min(axis) + 0.5 * (nodeBB.max(axis) - nodeBB.min(axis));

  auto newRightBB = nodeBB;
  auto newLeftBB = nodeBB;

  newRightBB.min(axis) = newLeftBB.max(axis) = sep;
  node->right.reset(new Node<T>{T{}, Axis::NONE, newRightBB});
  node->left.reset(new Node<T>{T{}, Axis::NONE, newLeftBB});

  auto indicies = node->indicies;
  node->indicies.clear();

  for (auto index : indicies)
    nonExpandingInsert(node, triangles_[index], index, /* isSubdiv = */ true);
}

//============================================================================================
//                             KdTree::ConstIterator definitions
//============================================================================================

template <std::floating_point T>
KdTree<T>::ConstIterator::ConstIterator(const KdTree<T> *tree, const Node<T> *node)
  : tree_(tree), node_(node), fifo_({node})
{}

template <std::floating_point T>
typename KdTree<T>::ConstIterator &KdTree<T>::ConstIterator::operator++()
{
  if (0 == fifo_.size())
    return *this;

  auto fifoEntry = fifo_.front();
  fifo_.pop();

  if (Axis::NONE != fifoEntry->sepAxis)
  {
    if (nullptr != fifoEntry->left)
      fifo_.push(fifoEntry->left.get());
    if (nullptr != fifoEntry->right)
      fifo_.push(fifoEntry->right.get());
  }

  node_ = (0 == fifo_.size()) ? nullptr : fifo_.front();
  return *this;
}

template <std::floating_point T>
typename KdTree<T>::ConstIterator KdTree<T>::ConstIterator::operator++(int)
{
  auto tmp = *this;
  operator++();
  return tmp;
}

template <std::floating_point T>
typename KdTree<T>::ConstIterator::reference KdTree<T>::ConstIterator::operator*() const
{
  return Container<T>{tree_, node_};
}

// template <std::floating_point T>
// typename KdTree<T>::ConstIterator::pointer KdTree<T>::ConstIterator::operator->() const
// {
//   return std::make_unique<Container<T>>(tree_, node_);
// }

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

template <std::floating_point T>
typename KdTree<T>::ConstIterator KdTree<T>::ConstIterator::beginFrom(
  const typename KdTree<T>::ConstIterator &iter)
{
  return ConstIterator{iter.tree_, iter.node_};
}

} // namespace geom::kdtree

#endif // __INCLUDE_KDTREE_KDTREE_HH__
