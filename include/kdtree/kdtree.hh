#ifndef __INCLUDE_KDTREE_KDTREE_HH__
#define __INCLUDE_KDTREE_KDTREE_HH__

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
    using reference = Container<T> &;
    using pointer = Container<T> *;

  private:
    KdTree<T> *tree_;
    Node<T> *node_;

  public:
    ConstIterator(const Node<T> *node);
    ConstIterator(const ConstIterator &iter);
    ConstIterator(ConstIterator &&iter);

    ConstIterator &operator=(const ConstIterator &cont);
    ConstIterator &operator=(ConstIterator &&cont);

    ~ConstIterator();

    void operator++();
    ConstIterator operator++(int);

    reference operator*() const;
    pointer operator->() const;

    bool operator==(const ConstIterator &lhs);
    bool operator!=(const ConstIterator &lhs);
  };
};

} // namespace geom::kdtree

#endif // __INCLUDE_KDTREE_KDTREE_HH__
