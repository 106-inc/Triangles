#include <iostream>
#include <vector>

#include "intersection/intersection.hh"
#include "primitives/primitives.hh"

int main()
{
  size_t n = 0;
  std::cin >> n;

  std::vector<geom::Triangle<double>> triangles{};
  std::vector<bool> isIntersect{};

  triangles.resize(n);
  isIntersect.resize(n);

  for (size_t i = 0; i < n; ++i)
    std::cin >> triangles[i];

  for (size_t i = 0; i < n; ++i)
    for (size_t j = i + 1; j < n; ++j)
      if (geom::isIntersect(triangles[i], triangles[j]))
        isIntersect[i] = isIntersect[j] = true;

  for (size_t i = 0; i < n; ++i)
    if (isIntersect[i])
      std::cout << i << std::endl;

  return 0;
}
