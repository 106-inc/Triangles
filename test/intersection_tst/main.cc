#include <gtest/gtest.h>

#include "intersection/intersection.hh"

using namespace geom;

TEST(Intersection, parallelTriangles1)
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

TEST(Intersection, parallelTriangles2)
{
  // Arrange
  Triangle<double> t1{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
  Triangle<double> t2{{2, 0, 0}, {0, 2, 0}, {0, 0, 2}};

  // Act & Assert
  ASSERT_FALSE(isIntersect(t1, t2));
}

TEST(Intersection, samePlanes)
{
  // Arrane
  auto pl1 = Plane<double>::getNormalDist({0, 0, 1}, 1);
  auto pl2 = Plane<double>::getNormalDist({0, 0, 1}, 1);

  // Act
  bool res = std::holds_alternative<Plane<double>>(intersect(pl1, pl2));

  // Assert
  ASSERT_TRUE(res);
}

TEST(Intersection, parallelPlanes)
{
  // Arrange
  auto pl1 = Plane<double>::getNormalDist({0, 0, 1}, 1);
  auto pl2 = Plane<double>::getNormalDist({0, 0, 1}, 2);

  // Act
  bool res = std::holds_alternative<std::monostate>(intersect(pl1, pl2));

  // Assert
  ASSERT_TRUE(res);
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}