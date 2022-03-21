#include <concepts>
#include <sstream>

#include <gtest/gtest.h>

#include "primitives/primitives.hh"

using namespace geom;

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

TEST(Plane, ctor)
{
  // Arrange
  auto pl1 = Plane<double>::getNormalDist({7, 0, 0}, 4);
  auto pl2 = Plane<double>::getParametric({0, 0, 1}, {1, 0, 0}, {0, 1, 0});
  auto pl3 = Plane<double>::getNormalPoint({0, 0, 1}, {1, 1, 1});
  auto pl4 = Plane<double>::getBy3Points({0, 0, 1}, {1, 1, 1}, {5, 2, 1});

  // Act & Assert
  checkCtor<double>(pl1, {1, 0, 0}, 4);
  checkCtor<double>(pl2, {0, 0, 1}, 1);
  checkCtor<double>(pl3, {0, 0, 1}, 1);
  checkCtor<double>(pl4, {0, 0, -1}, -1);
}

TEST(Plane, copyCtor)
{
  // Arrange
  auto pl1 = Plane<double>::getNormalDist({7, 0, 0}, 4);
  auto pl2 = pl1;
  auto pl3{pl1};

  // Act & Assert
  EXPECT_EQ(pl1, pl2);
  EXPECT_EQ(pl1, pl3);
}

TEST(Plane, belongsPoint)
{
  // Arrange
  auto pl1 = Plane<double>::getParametric({1, 0, 0}, {-1, 0, 1}, {-1, -1, 0});
  auto pl2 = Plane<double>::getNormalPoint({-1, -1, -1}, {0, 0, 0});

  // Act
  auto resTrue1 = pl1.belongs(Vec3D{0, 0, 1});
  auto resFalse1 = pl1.belongs(Vec3D{0, 0, 2});

  auto resTrue2 = pl2.belongs(Vec3D{1, 1, -2});
  auto resFalse2 = pl2.belongs(Vec3D{1, 1, 1});

  // Assert
  EXPECT_TRUE(resTrue1);
  EXPECT_FALSE(resFalse1);

  EXPECT_TRUE(resTrue2);
  EXPECT_FALSE(resFalse2);
}

TEST(Plane, belongsLine)
{
  // Arrange
  auto pl = Plane<double>::getParametric({1, 1, 1}, {1, 2, 1}, {2, 1, 1});
  Line<double> l1{{1, 1, 1}, {1, 2, 1}};
  Line<double> l2{{2, 2, 2}, {1, 2, 1}};

  // Act & Assert
  EXPECT_TRUE(pl.belongs(l1));
  EXPECT_FALSE(pl.belongs(l2));
}

TEST(Plane, isEqual)
{
  // Arrange
  auto pl1 = Plane<double>::getNormalDist({3, 4, 5}, 17);
  auto pl2 = Plane<double>::getNormalDist({-3, -4, -5}, -17);
  auto pl3 = Plane<double>::getNormalDist({3, 4, 5}, -17);
  auto pl4 = Plane<double>::getNormalDist({-3, -4, -5}, 17);

  // Act & Assert
  EXPECT_EQ(pl1, pl2);
  EXPECT_EQ(pl3, pl4);

  EXPECT_NE(pl1, pl3);
  EXPECT_NE(pl1, pl4);
  EXPECT_NE(pl2, pl3);
  EXPECT_NE(pl2, pl4);
}

TEST(Plane, output)
{
  // Arrange
  auto pl = Plane<double>::getNormalDist({2, 0, 0}, 17);

  // Act
  std::stringstream ss{};
  ss << pl;

  // Assert
  EXPECT_EQ(ss.str(), "(1, 0, 0) * X = 17");
}

TEST(Plane, isPar)
{
  // Arrange
  auto pl1 = Plane<double>::getBy3Points({0, 0, 0}, {0, 0, 1}, {0, 1, 0});
  auto pl2 = Plane<double>::getBy3Points({1, 0, 0}, {1, 0, 1}, {1, 1, 0});
  auto pl3 = Plane<double>::getBy3Points({1, 1, 1}, {0, 0, 1}, {0, 1, 0});

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