#include <iostream>
#include <set>
#include <stack>

#include "intersection/intersection.hh"
#include "kdtree/kdtree.hh"

using namespace geom;
using namespace geom::kdtree;

template <std::floating_point T>
void selfIntersect(const Container<T> &node, std::set<Index> &intInd)
{
  auto end = node.end();
  for (auto i = node.begin(); i != end; ++i)
    for (auto j = std::next(i); j != end; ++j)
      if (isIntersect(*i, *j))
        intInd.insert(i.getIndex()), intInd.insert(j.getIndex());
}

template <std::floating_point T>
void intersect(typename Container<T>::ConstIterator &trIt, std::set<Index> &intInd,
               const Container<T> &node)
{
  auto left = node.left();
  auto right = node.right();

  std::stack<Container<T>> stack{};

  if (left.isValid())
    stack.push(left);

  if (right.isValid())
    stack.push(right);

  while (!stack.empty())
  {
    auto curNode = stack.top();
    stack.pop();

    for (auto i = curNode.begin(), end = curNode.end(); i != end; ++i)
      if (isIntersect(*i, *trIt))
        intInd.insert(i.getIndex()), intInd.insert(trIt.getIndex());

    if (KdTree<T>::isOnPosSide(curNode.sepAxis(), curNode.separator(), *trIt))
      stack.push(curNode.right());
    else if (KdTree<T>::isOnNegSide(curNode.sepAxis(), curNode.separator(), *trIt))
      stack.push(curNode.left());
    else if (curNode.sepAxis() != Axis::NONE)
    {
      stack.push(curNode.left());
      stack.push(curNode.right());
    }
  }
}

int main()
{
  std::size_t n = 0;
  std::cin >> n;
  std::set<Index> intersectIndicies{};

  KdTree<float> tree{};
  for (std::size_t i = 0; i < n; ++i)
  {
    Triangle<float> tr{};
    std::cin >> tr;
    tree.insert(tr);
  }

  for (auto cont : tree)
  {
    selfIntersect(cont, intersectIndicies);
    for (auto trIt = cont.begin(), trEnd = cont.end(); trIt != trEnd; ++trIt)
      intersect<float>(trIt, intersectIndicies, cont);
  }

  for (auto elem : intersectIndicies)
    std::cout << elem << std::endl;
}
