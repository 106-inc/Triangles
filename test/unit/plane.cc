#include <concepts>
#include <sstream>

#include <gtest/gtest.h>

#include "primitives/primitives.hh"

using namespace geom;
using FPTypes = testing::Types<float, double, long double>;

template <typename T>
class PlaneTest : public testing::Test
{};

TYPED_TEST_SUITE(PlaneTest, FPTypes);

template <std::floating_point T>
static void checkCtor(const Plane<T> &pl, const Vec3<T> &normExpected, T distExpected)
{
  // Act
  auto norm = pl.norm();
  auto dist = pl.dist();

  // Assert
  EXPECT_TRUE(normExpected.isEqual(norm));
  EXPECT_TRUE(Vec3<T>::isNumEq(distExpected, dist));
}

TYPED_TEST(PlaneTest, ctor)
{
  // Arrange
  auto pl1 = Plane<TypeParam>::getNormalDist({7, 0, 0}, 4);
  auto pl2 = Plane<TypeParam>::getParametric({0, 0, 1}, {1, 0, 0}, {0, 1, 0});
  auto pl3 = Plane<TypeParam>::getNormalPoint({0, 0, 1}, {1, 1, 1});
  auto pl4 = Plane<TypeParam>::getBy3Points({0, 0, 1}, {1, 1, 1}, {5, 2, 1});

  // Act & Assert
  checkCtor<TypeParam>(pl1, {1, 0, 0}, 4);
  checkCtor<TypeParam>(pl2, {0, 0, 1}, 1);
  checkCtor<TypeParam>(pl3, {0, 0, 1}, 1);
  checkCtor<TypeParam>(pl4, {0, 0, -1}, -1);
}

TYPED_TEST(PlaneTest, copyCtor)
{
  // Arrange
  auto pl1 = Plane<TypeParam>::getNormalDist({7, 0, 0}, 4);
  auto pl2 = pl1;
  auto pl3{pl1};

  // Act & Assert
  EXPECT_EQ(pl1, pl2);
  EXPECT_EQ(pl1, pl3);
}

TYPED_TEST(PlaneTest, belongsPoint)
{
  // Arrange
  auto pl1 = Plane<TypeParam>::getParametric({1, 0, 0}, {-1, 0, 1}, {-1, -1, 0});
  auto pl2 = Plane<TypeParam>::getNormalPoint({-1, -1, -1}, {0, 0, 0});

  // Act
  auto resTrue1 = pl1.belongs(Vec3<TypeParam>{0, 0, 1});
  auto resFalse1 = pl1.belongs(Vec3<TypeParam>{0, 0, 2});

  auto resTrue2 = pl2.belongs(Vec3<TypeParam>{1, 1, -2});
  auto resFalse2 = pl2.belongs(Vec3<TypeParam>{1, 1, 1});

  // Assert
  EXPECT_TRUE(resTrue1);
  EXPECT_FALSE(resFalse1);

  EXPECT_TRUE(resTrue2);
  EXPECT_FALSE(resFalse2);
}

TYPED_TEST(PlaneTest, belongsLine)
{
  // Arrange
  auto pl = Plane<TypeParam>::getParametric({1, 1, 1}, {1, 2, 1}, {2, 1, 1});
  Line<TypeParam> l1{{1, 1, 1}, {1, 2, 1}};
  Line<TypeParam> l2{{2, 2, 2}, {1, 2, 1}};

  // Act & Assert
  EXPECT_TRUE(pl.belongs(l1));
  EXPECT_FALSE(pl.belongs(l2));
}

TYPED_TEST(PlaneTest, isEqual)
{
  // Arrange
  auto pl1 = Plane<TypeParam>::getNormalDist({3, 4, 5}, 17);
  auto pl2 = Plane<TypeParam>::getNormalDist({-3, -4, -5}, -17);
  auto pl3 = Plane<TypeParam>::getNormalDist({3, 4, 5}, -17);
  auto pl4 = Plane<TypeParam>::getNormalDist({-3, -4, -5}, 17);

  // Act & Assert
  EXPECT_EQ(pl1, pl2);
  EXPECT_EQ(pl3, pl4);

  EXPECT_NE(pl1, pl3);
  EXPECT_NE(pl1, pl4);
  EXPECT_NE(pl2, pl3);
  EXPECT_NE(pl2, pl4);
}

TYPED_TEST(PlaneTest, output)
{
  // Arrange
  auto pl = Plane<TypeParam>::getNormalDist({2, 0, 0}, 17);

  // Act
  std::stringstream ss{};
  ss << pl;

  // Assert
  EXPECT_EQ(ss.str(), "(1, 0, 0) * X = 17");
}

TYPED_TEST(PlaneTest, isPar)
{
  // Arrange
  auto pl1 = Plane<TypeParam>::getBy3Points({0, 0, 0}, {0, 0, 1}, {0, 1, 0});
  auto pl2 = Plane<TypeParam>::getBy3Points({1, 0, 0}, {1, 0, 1}, {1, 1, 0});
  auto pl3 = Plane<TypeParam>::getBy3Points({1, 1, 1}, {0, 0, 1}, {0, 1, 0});

  // Act & Assert
  EXPECT_TRUE(pl1.isPar(pl2));
  EXPECT_TRUE(pl2.isPar(pl1));

  EXPECT_FALSE(pl1.isPar(pl3));
  EXPECT_FALSE(pl2.isPar(pl3));
  EXPECT_FALSE(pl3.isPar(pl1));
  EXPECT_FALSE(pl3.isPar(pl2));

  EXPECT_TRUE(pl1.isPar(pl1));
  EXPECT_TRUE(pl2.isPar(pl2));
  EXPECT_TRUE(pl3.isPar(pl3));
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
