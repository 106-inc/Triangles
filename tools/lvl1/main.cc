#include <iostream>

#include "kdtree/kdtree.hh"

using namespace geom;
using namespace geom::kdtree;

int main()
{
  KdTree<float> tree{};

  tree.insert({{0, 0, 0}, {1, 0, 0}, {0, 1, 0}});
  tree.insert({{0, 0, 0}, {-1, 0, 0}, {0, -1, 0}});
  tree.insert({{0, 0, 0}, {0.7, 0, 0}, {0, 0.7, 0}});
  tree.dump();

  return 0;
}
