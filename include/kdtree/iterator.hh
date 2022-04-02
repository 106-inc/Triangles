#ifndef __INCLUDE_KDTREE_ITERATOR_HH__
#define __INCLUDE_KDTREE_ITERATOR_HH__

#include <iterator>

#include "node.hh"

namespace geom
{
namespace kdtree
{

template <std::floating_point T>
class Iterator
  : public std::iterator<std::forward_iterator_tag, const Node<T>, void, const Node<T> *, const Node<T> &>
{
private:
  /* data */
public:
  Iterator(/* args */);
  ~Iterator();
};

Iterator::Iterator(/* args */)
{}

Iterator::~Iterator()
{}

} // namespace kdtree
} // namespace geom

#endif // __INCLUDE_KDTREE_ITERATOR_HH__
