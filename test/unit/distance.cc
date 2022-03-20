#include <gtest/gtest.h>

#include "distance/distance.hh"

using namespace geom;

TEST(distance, PlanePoint1)
{
  // Arrange
  auto pl = Plane<double>::getNormalDist({0, 0, 1}, 0);
  Vec3<double> pt{0, 0, 1};

  // Act
  auto dist = distance(pl, pt);

  // Assert
  ASSERT_DOUBLE_EQ(dist, 1);
}

TEST(distance, PlanePoint2)
{
  // Arrange
  auto pl = Plane<double>::getNormalDist({5, 67, 3}, 7);
  Vec3<double> pt{0, 0, 0};

  // Act
  auto dist = distance(pl, pt);

  // Assert
  ASSERT_DOUBLE_EQ(dist, -7);
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
