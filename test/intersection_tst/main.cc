#include <gtest/gtest.h>

#include "intersection/intersection.hh"

using namespace geom;

TEST(Intersection, parallel1)
{
  // Arrange
  Triangle<double> t1{{0, 0, 0}, {0, 1, 0}, {0, 0, 1}};
  Triangle<double> t2{{1, 0, 0}, {1, 1, 0}, {1, 0, 1}};
  Triangle<double> t3{{1e-4, 0, 0}, {1e-4, 1, 0}, {1e-4, 0, 1}};

  // Act & Assert
  ASSERT_FALSE(isIntersect(t1, t2));
  ASSERT_FALSE(isIntersect(t1, t3));
  ASSERT_FALSE(isIntersect(t2, t3));
}

TEST(Intersection, parallel2)
{
  // Arrange
  Triangle<double> t1{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
  Triangle<double> t2{{2, 0, 0}, {0, 2, 0}, {0, 0, 2}};

  // Act & Assert
  ASSERT_FALSE(isIntersect(t1, t2));
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}