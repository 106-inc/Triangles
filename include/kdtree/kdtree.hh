#ifndef __INCLUDE_KDTREE_KDTREE_HH__
#define __INCLUDE_KDTREE_KDTREE_HH__

#include <initializer_list>
#include <memory>
#include <vector>

#include "primitives/primitives.hh"

#include "node.hh"

namespace geom::kdtree
{

using Iterator = void;

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

  // Iterators
  Iterator begin();
  Iterator end();

  // Modifiers
  Iterator insert(const Triangle<T> &tr);
  void clear();

  // Capacity
  bool empty();
  size_t size();

  class Iterator final
  {
  public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::size_t;
    using value_type = Container<T>;
    using reference = Container<T> &;
    using pointer = Container<T> *; // ???

  private:
    KdTree<T> *tree_;
    Node<T> *node_;

  public:
    Iterator(const Node<T> *node);
    Iterator(const Iterator &iter);
    Iterator(Iterator &&iter);

    Iterator &operator=(const Iterator &cont);
    Iterator &operator=(Iterator &&cont);

    ~Iterator();

    void operator++();
    Iterator operator++(int);

    reference operator*() const;
    pointer operator->() const;

    bool operator==(const Iterator &lhs);
    bool operator!=(const Iterator &lhs);
  };
};

} // namespace geom::kdtree

#endif // __INCLUDE_KDTREE_KDTREE_HH__
