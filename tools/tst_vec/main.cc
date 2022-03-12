#include <iostream>

#include "primitives/vec3.hh"

int main()
{
  geom::Vec3<float> a{3, 4, 5};
  std::cout << a << std::endl;

  auto b = a;
  std::cout << b << std::endl;

  std::cout << a.isEqual(b) << std::endl;
  return 0;
}
