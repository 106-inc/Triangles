#include <iostream>

#include "geom/vector.hh"

int main()
{
  geom::Vector<float> a{3, 4, 5};
  std::cout << a << std::endl;

  auto b = a;
  std::cout << b << std::endl;
  return 0;
}
