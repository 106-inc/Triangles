#include <gtest/gtest.h>

#include "primitives/triangle.hh"

using namespace geom;

TEST(Triangle, ctor)
{
  // Arrange
  VectorD v1{1, 2, 3};
  VectorD v2{4, 5, 6};
  VectorD v3{7, 8, 9};
  Triangle tr{v1, v2, v3};

  // Act & Assert
  ASSERT_EQ(tr[0], v1);
  ASSERT_EQ(tr[1], v2);
  ASSERT_EQ(tr[2], v3);
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
