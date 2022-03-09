#include <gtest/gtest.h>

#include "intersection/intersection.hh"

using namespace geom;

TEST(intersection, TrianglesParallel1)
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

TEST(intersection, TrianglesParallel2)
{
  // Arrange
  Triangle<double> t1{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
  Triangle<double> t2{{2, 0, 0}, {0, 2, 0}, {0, 0, 2}};

  // Act & Assert
  ASSERT_FALSE(isIntersect(t1, t2));
  ASSERT_FALSE(isIntersect(t2, t1));
}

TEST(intersection, TrianglesSameSide)
{
  // Arrange
  Triangle<double> t1{{0, 0, 0}, {0, 1, 0}, {1, 0, 0}};
  Triangle<double> t2{{0, 0, 1}, {0, 1, 1}, {1, 0, 2}};

  // Act & Assert
  ASSERT_FALSE(isIntersect(t1, t2));
  ASSERT_FALSE(isIntersect(t2, t1));
}

TEST(intersection, commonCase)
{
  // Arrange
  Triangle<double> t1{{0, 0, 0}, {0, 1, 0}, {1, 0, 0}};
  Triangle<double> t2{{0, 0, 1}, {0, 1, -1}, {1, 0, -1}};

  // Act & Assert
  ASSERT_TRUE(isIntersect(t1, t2));
}

TEST(intersection, PlanesSame)
{
  // Arrane
  auto pl1 = Plane<double>::getNormalDist({0, 0, 1}, 1);
  auto pl2 = Plane<double>::getNormalDist({0, 0, 1}, 1);

  // Act
  bool res = std::holds_alternative<Plane<double>>(intersect(pl1, pl2));

  // Assert
  ASSERT_TRUE(res);
}

TEST(intersection, PlanesParallel)
{
  // Arrange
  auto pl1 = Plane<double>::getNormalDist({0, 0, 1}, 1);
  auto pl2 = Plane<double>::getNormalDist({0, 0, 1}, 2);

  // Act
  bool res = std::holds_alternative<std::monostate>(intersect(pl1, pl2));

  // Assert
  ASSERT_TRUE(res);
}

TEST(intersection, Planes)
{
  // Arrange
  auto pl1 = Plane<double>::getBy3Points({0, 0, 0}, {1, 0, 0}, {0, 1, 0});
  auto pl2 = Plane<double>::getBy3Points({0, 0, 0}, {0, 0, 1}, {0, 1, 0});
  auto pl3 = Plane<double>::getBy3Points({0, 0, 0}, {0, 0, 1}, {1, 1, 0});

  Line<double> l1{{0, 0, 0}, {0, 1, 0}};
  Line<double> l2{{0, 0, 0}, {1, 1, 0}};

  // Act
  auto res12 = std::get<Line<double>>(intersect(pl1, pl2));
  auto res13 = std::get<Line<double>>(intersect(pl1, pl3));

  // Assert
  ASSERT_EQ(l1, res12);
  ASSERT_EQ(l2, res13);
}

TEST(intersection, intervalOverlap)
{
  // Arrange
  std::pair<double, double> interv1{-10, 10};

  std::pair<double, double> interv2{-5, 5};
  std::pair<double, double> interv3{-11, 11};

  std::pair<double, double> interv4{0, 11};
  std::pair<double, double> interv5{-11, 0};

  std::pair<double, double> interv6{11, 12};
  std::pair<double, double> interv7{-12, -11};

  // Act & Assert
  ASSERT_TRUE(detail::isOverlap(interv1, interv2));
  ASSERT_TRUE(detail::isOverlap(interv1, interv3));
  ASSERT_TRUE(detail::isOverlap(interv2, interv1));
  ASSERT_TRUE(detail::isOverlap(interv3, interv1));

  ASSERT_TRUE(detail::isOverlap(interv1, interv4));
  ASSERT_TRUE(detail::isOverlap(interv1, interv5));
  ASSERT_TRUE(detail::isOverlap(interv4, interv1));
  ASSERT_TRUE(detail::isOverlap(interv5, interv1));

  ASSERT_FALSE(detail::isOverlap(interv1, interv6));
  ASSERT_FALSE(detail::isOverlap(interv1, interv7));
  ASSERT_FALSE(detail::isOverlap(interv6, interv1));
  ASSERT_FALSE(detail::isOverlap(interv7, interv1));
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}