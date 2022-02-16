#include <concepts>
#include <sstream>

#include <gtest/gtest.h>

#include "primitives/plane.hh"

using namespace geom;

template <std::floating_point T>
static void checkCtor(const Plane<T> &p, const Vector<T> &normExpected, T distExpected)
{
  // Act
  auto norm = p.norm();
  auto dist = p.dist();

  // Assert
  ASSERT_TRUE(normExpected.isEqual(norm));
  ASSERT_TRUE(Vector<T>::isNumEq(distExpected, dist));
}

TEST(Plane, ctor)
{
  // Arrange
  auto p1 = Plane<double>::getNormalDist({7, 0, 0}, 4);
  auto p2 = Plane<double>::getParametric({0, 0, 1}, {1, 0, 0}, {0, 1, 0});
  auto p3 = Plane<double>::getNormalPoint({0, 0, 1}, {1, 1, 1});
  auto p4 = Plane<double>::getBy3Points({0, 0, 1}, {1, 1, 1}, {5, 2, 1});

  // Act & Assert
  checkCtor<double>(p1, {1, 0, 0}, 4);
  checkCtor<double>(p2, {0, 0, 1}, 1);
  checkCtor<double>(p3, {0, 0, 1}, 1);
  checkCtor<double>(p4, {0, 0, -1}, -1);
}

TEST(Plane, belongsPoint)
{
  // Arrange
  auto p1 = Plane<double>::getParametric({1, 0, 0}, {-1, 0, 1}, {-1, -1, 0});
  auto p2 = Plane<double>::getNormalPoint({-1, -1, -1}, {0, 0, 0});

  // Act
  auto resTrue1 = p1.belongs(VectorD{0, 0, 1});
  auto resFalse1 = p1.belongs(VectorD{0, 0, 2});

  auto resTrue2 = p2.belongs(VectorD{1, 1, -2});
  auto resFalse2 = p2.belongs(VectorD{1, 1, 1});

  // Assert
  ASSERT_TRUE(resTrue1);
  ASSERT_FALSE(resFalse1);

  ASSERT_TRUE(resTrue2);
  ASSERT_FALSE(resFalse2);
}

TEST(Plane, belongsLine)
{
  // Arrange
  auto p = Plane<double>::getParametric({1, 1, 1}, {1, 2, 1}, {2, 1, 1});
  Line<double> l1{{1, 1, 1}, {1, 2, 1}};
  Line<double> l2{{2, 2, 2}, {1, 2, 1}};

  // Act & Assert
  ASSERT_TRUE(p.belongs(l1));
  ASSERT_FALSE(p.belongs(l2));
}

TEST(Plane, isEqual)
{
  // Arrange
  auto p1 = Plane<double>::getNormalDist({3, 4, 5}, 17);
  auto p2 = Plane<double>::getNormalDist({-3, -4, -5}, -17);
  auto p3 = Plane<double>::getNormalDist({3, 4, 5}, -17);
  auto p4 = Plane<double>::getNormalDist({-3, -4, -5}, 17);

  // Act & Assert
  ASSERT_EQ(p1, p2);
  ASSERT_EQ(p3, p4);

  ASSERT_NE(p1, p3);
  ASSERT_NE(p1, p4);
  ASSERT_NE(p2, p3);
  ASSERT_NE(p2, p4);
}

TEST(Plane, ostream)
{
  // Arrange
  auto p = Plane<double>::getNormalDist({2, 0, 0}, 17);

  // Act
  std::stringstream ss{};
  ss << p;

  // Assert
  ASSERT_EQ(ss.str(), "(1, 0, 0) * X = 17");
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
