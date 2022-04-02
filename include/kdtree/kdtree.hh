#ifndef __INCLUDE_KDTREE_KDTREE_HH__
#define __INCLUDE_KDTREE_KDTREE_HH__

#include <vector>
#include <memory>

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
  KdTree(/* args */);
  ~KdTree();

  Iterator insert(const Triangle<T> &tr);
  Iterator begin();
  Iterator end();
};

template <std::floating_point T>
KdTree<T>::KdTree(/* args */)
{}

template <std::floating_point T>
KdTree<T>::~KdTree()
{}

} // namespace kdtree
} // namespace geom

#endif // __INCLUDE_KDTREE_KDTREE_HH__
