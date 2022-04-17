#include <iostream>
#include <set>

#include "intersection/intersection.hh"
#include "kdtree/kdtree.hh"

using namespace geom;
using namespace geom::kdtree;

void selfIntersect(const Container<float> &node, std::set<Index> &intInd)
{
  auto end = node.end();
  for (auto i = node.begin(); i != end; ++i)
    for (auto j = std::next(i); j != end; ++j)
      if (isIntersect(*i, *j))
        intInd.insert(i.getIndex()), intInd.insert(j.getIndex());
}

void intersect(const Container<float> &node1, const Container<float> &node2,
               std::set<Index> &intInd)
{
  auto end1 = node1.end();
  auto end2 = node2.end();
  for (auto i = node1.begin(); i != end1; ++i)
    for (auto j = node2.begin(); j != end2; ++j)
      if (isIntersect(*i, *j))
        intInd.insert(i.getIndex()), intInd.insert(j.getIndex());
}

int main()
{
  size_t n = 0;
  std::cin >> n;
  std::set<Index> intersectIndicies{};

  KdTree<float> tree{};
  for (size_t i = 0; i < n; ++i)
  {
    Triangle<float> tr{};
    std::cin >> tr;
    tree.insert(tr);
  }

  for (auto nodeIt = tree.begin(), nodeEnd = tree.end(); nodeIt != nodeEnd; ++nodeIt)
  {
    selfIntersect(*nodeIt, intersectIndicies);
    for (auto it = std::next(tree.beginFrom(nodeIt)); it != nodeEnd; ++it)
      intersect(*nodeIt, *it, intersectIndicies);
  }

  for (auto elem : intersectIndicies)
    std::cout << elem << std::endl;

  // tree.dumpRecursive();
}
