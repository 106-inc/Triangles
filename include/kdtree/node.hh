#ifndef __INCLUDE_KDTREE_NODE_HH__
#define __INCLUDE_KDTREE_NODE_HH__

#include <memory>
#include <vector>

#include "primitives/boundbox.hh"
#include "primitives/primitives.hh"

namespace geom::kdtree
{

enum class Axis : int8_t
{
  x = 0,
  y = 1,
  z = 2,
  none
};

using Index = size_t;

template <std::floating_point T>
struct Node final
{
  T separator{};            // separator's coordinate on separation axis
  Axis sepAxis{Axis::none}; // separation axis
  BoundBox<T> boundBox{};
  std::vector<Index> indicies{};

  std::unique_ptr<Node> left;
  std::unique_ptr<Node> right;
};

} // namespace geom::kdtree

#endif // __INCLUDE_KDTREE_NODE_HH__
