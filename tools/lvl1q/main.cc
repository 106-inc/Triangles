#include <iostream>
#include <vector>

#include "geom/intersection/intersection.hh"
#include "geom/primitives/primitives.hh"

int main()
{
  std::size_t n = 0;
  std::cin >> n;

  std::vector<geom::Triangle<double>> triangles{};
  std::vector<bool> isIntersect{};

  triangles.resize(n);
  isIntersect.resize(n);

  for (auto &tr : triangles)
    std::cin >> tr;

  for (std::size_t i = 0; i < n; ++i)
    for (std::size_t j = i + 1; j < n; ++j)
      if (geom::isIntersect(triangles[i], triangles[j]))
        isIntersect[i] = isIntersect[j] = true;

  for (std::size_t i = 0; i < n; ++i)
    if (isIntersect[i])
      std::cout << i << std::endl;

  return 0;
}
