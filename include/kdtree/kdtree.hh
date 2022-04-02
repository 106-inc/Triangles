#ifndef __INCLUDE_KDTREE_KDTREE_HH__
#define __INCLUDE_KDTREE_KDTREE_HH__

#include <initializer_list>
#include <memory>
#include <vector>

#include "primitives/primitives.hh"

#include "node.hh"

namespace geom
{
namespace kdtree
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
};

} // namespace kdtree
} // namespace geom

#endif // __INCLUDE_KDTREE_KDTREE_HH__
