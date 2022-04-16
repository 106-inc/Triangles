#include <iostream>

#include "kdtree/kdtree.hh"

using namespace geom;
using namespace geom::kdtree;

int oldMain()
{
  KdTree<float> tree{};

  tree.insert({{0, 0, 0}, {1, 0, 0}, {0, 1, 0}});        // 0
  tree.insert({{0, 0, 0}, {-1, 0, 0}, {0, -1, 0}});      // 1
  tree.insert({{0, 0, 0}, {0.7, 0, 0}, {0, 0.7, 0}});    // 2
  tree.insert({{-0.5, 0, 0}, {0.5, 0, 0}, {0, 0.1, 0}}); // 3
  tree.dumpRecursive();

  return 0;
}

int main()
{
  size_t n = 0;
  std::cin >> n;

  KdTree<float> tree{};
  for (size_t i = 0; i < n; ++i)
  {
    Triangle<float> tr{};
    std::cerr << i << std::endl;
    std::cin >> tr;
    tree.insert(tr);
  }

  tree.dumpRecursive();
}
