#include <gtest/gtest.h>

#include "distance/distance.hh"

using namespace geom;
using FPTypes = testing::Types<float, double, long double>;

template <typename T>
class DistanceTest : public testing::Test
{};

TYPED_TEST_SUITE(DistanceTest, FPTypes);

TYPED_TEST(DistanceTest, PlanePoint1)
{
  // Arrange
  auto pl = Plane<TypeParam>::getNormalDist({0, 0, 1}, 0);
  Vec3<TypeParam> pt{0, 0, 1};

  // Act
  auto dist = distance(pl, pt);

  // Assert
  EXPECT_NEAR(dist, 1, Vec3<TypeParam>::getThreshold());
}

TYPED_TEST(DistanceTest, PlanePoint2)
{
  // Arrange
  auto pl = Plane<TypeParam>::getNormalDist({5, 67, 3}, 7);
  Vec3<TypeParam> pt{0, 0, 0};

  // Act
  auto dist = distance(pl, pt);

  // Assert
  EXPECT_NEAR(dist, -7, Vec3<TypeParam>::getThreshold());
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
