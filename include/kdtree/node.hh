#ifndef __INCLUDE_KDTREE_NODE_HH__
#define __INCLUDE_KDTREE_NODE_HH__

#include <memory>
#include <vector>

#include "primitives/primitives.hh"

namespace geom::kdtree
{

using Index = size_t;

template <std::floating_point T>
struct Node final
{
  T separator{};            // separator's coordinate on separation axis
  Axis sepAxis{Axis::NONE}; // separation axis
  BoundBox<T> boundBox{};
  std::vector<Index> indicies{};

  std::unique_ptr<Node> left{nullptr};
  std::unique_ptr<Node> right{nullptr};
};

} // namespace geom::kdtree

#endif // __INCLUDE_KDTREE_NODE_HH__
