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

  // std::for_each(tree.begin(), tree.end(), [](const auto &cont) {
  //   std::cerr << "axis:" << static_cast<int>(cont.sepAxis()) << "; " << cont.boundBox()
  //             << std::endl;
  // });

  for (const auto &cont : tree)
    std::cerr << "axis:" << static_cast<int>(cont.sepAxis()) << "; " << cont.boundBox()
              << std::endl;

  for (const auto &cont : tree)
    for (const auto &tr : cont)
      std::cerr << tr << std::endl;

  // for (auto it = tree.begin(); it != tree.end(); ++it)
  //   std::cerr << "axis:" << static_cast<int>(it->sepAxis()) << "; " << it->boundBox() <<
  //   std::endl;

  tree.dumpRecursive();
}
