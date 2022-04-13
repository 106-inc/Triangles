#include <iostream>

#include "kdtree/kdtree.hh"

using namespace geom;
using namespace geom::kdtree;

int main()
{
  KdTree<float> tree{};

  tree.insert({{0, 0, 0}, {1, 0, 0}, {0, 1, 0}});        // 0
  tree.insert({{0, 0, 0}, {-1, 0, 0}, {0, -1, 0}});      // 1
  tree.insert({{0, 0, 0}, {0.7, 0, 0}, {0, 0.7, 0}});    // 2
  tree.insert({{-0.5, 0, 0}, {0.5, 0, 0}, {0, 0.1, 0}}); // 3
  tree.dumpRecursive();

  return 0;
}
