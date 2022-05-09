#ifndef __INCLUDE_KDTREE_NODE_HH__
#define __INCLUDE_KDTREE_NODE_HH__

#include <iostream>
#include <memory>
#include <vector>

#include "primitives/primitives.hh"

namespace geom::kdtree
{

using Index = std::size_t;

template <std::floating_point T>
struct Node final
{
  T separator{};            // separator's coordinate on separation axis
  Axis sepAxis{Axis::NONE}; // separation axis
  BoundBox<T> boundBox{};
  std::vector<Index> indicies{};

  std::unique_ptr<Node> left{nullptr};
  std::unique_ptr<Node> right{nullptr};

  using IndexIterator = std::vector<Index>::iterator;
  using IndexConstIterator = std::vector<Index>::const_iterator;

  void dumpRecursive(std::ostream &ost) const;
};

template <std::floating_point T>
void Node<T>::dumpRecursive(std::ostream &ost) const
{
  ost << reinterpret_cast<std::uintptr_t>(this)
      << " [shape=box,label=\"axis: " << static_cast<int>(sepAxis) << ",\\n"
      << boundBox << ",\\nvec: {";

  for (auto elem : indicies)
    ost << elem << " ";

  ost << "}\"];" << std::endl;

  if (left)
  {
    left->dumpRecursive(ost);
    ost << reinterpret_cast<std::uintptr_t>(this) << " -> "
        << reinterpret_cast<std::uintptr_t>(left.get()) << " [label=\"L\"];" << std::endl;
  }
  if (right)
  {
    right->dumpRecursive(ost);
    ost << reinterpret_cast<std::uintptr_t>(this) << " -> "
        << reinterpret_cast<std::uintptr_t>(right.get()) << " [label=\"R\"];" << std::endl;
  }
}

} // namespace geom::kdtree

#endif // __INCLUDE_KDTREE_NODE_HH__
