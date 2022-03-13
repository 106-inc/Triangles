#include <gtest/gtest.h>

#include "intersection/intersection.hh"

using namespace geom;

TEST(triangles, Parallel1)
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

TEST(triangles, Parallel2)
{
  // Arrange
  Triangle<double> t1{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
  Triangle<double> t2{{2, 0, 0}, {0, 2, 0}, {0, 0, 2}};

  // Act & Assert
  ASSERT_FALSE(isIntersect(t1, t2));
  ASSERT_FALSE(isIntersect(t2, t1));
}

TEST(triangles, SamePlane)
{
  // Arrange
  Triangle<double> t1{{1, 0, 0}, {0, 1, 0}, {0, 0, 0}};
  Triangle<double> t2{{3, 0, 0}, {0, 3, 0}, {2, 2, 0}};
  Triangle<double> t3{{3, 0, 0}, {0, 3, 0}, {0, 0, 0}};

  // Act & Assert
  EXPECT_FALSE(isIntersect(t1, t2));
  EXPECT_FALSE(isIntersect(t2, t1));

  EXPECT_TRUE(isIntersect(t1, t3));
  EXPECT_TRUE(isIntersect(t3, t1));

  EXPECT_TRUE(isIntersect(t2, t3));
  EXPECT_TRUE(isIntersect(t3, t2));
}

TEST(triangles, SameSide)
{
  // Arrange
  Triangle<double> t1{{0, 0, 0}, {0, 1, 0}, {1, 0, 0}};
  Triangle<double> t2{{0, 0, 1}, {0, 1, 1}, {1, 0, 2}};

  // Act & Assert
  ASSERT_FALSE(isIntersect(t1, t2));
  ASSERT_FALSE(isIntersect(t2, t1));
}

TEST(triangles, CommonCase)
{
  // Arrange
  Triangle<double> t1{{0, 0, 0}, {0, 1, 0}, {1, 0, 0}};
  Triangle<double> t2{{0, 0, 1}, {0, 1, -1}, {1, 0, -1}};

  // Act & Assert
  ASSERT_TRUE(isIntersect(t1, t2));
}

TEST(planes, Same)
{
  // Arrane
  auto pl1 = Plane<double>::getNormalDist({0, 0, 1}, 1);
  auto pl2 = Plane<double>::getNormalDist({0, 0, 1}, 1);

  // Act
  bool res = std::holds_alternative<Plane<double>>(intersect(pl1, pl2));

  // Assert
  ASSERT_TRUE(res);
}

TEST(planes, Parallel)
{
  // Arrange
  auto pl1 = Plane<double>::getNormalDist({0, 0, 1}, 1);
  auto pl2 = Plane<double>::getNormalDist({0, 0, 1}, 2);

  // Act
  bool res = std::holds_alternative<std::monostate>(intersect(pl1, pl2));

  // Assert
  ASSERT_TRUE(res);
}

TEST(planes, Common)
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

TEST(detail, intervalOverlap)
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

TEST(detail, isSameSign)
{
  // Arrange
  std::vector<double> arr1{1.0, 34.0, 5.0, 2.0};
  std::vector<double> arr2{-1.0, 34.0, 5.0, 2.0};
  std::vector<double> arr3{0.0, 34.0, 5.0, 2.0};

  // Act & Assert
  ASSERT_TRUE(detail::isSameSign(arr1.begin(), arr1.end()));
  ASSERT_FALSE(detail::isSameSign(arr2.begin(), arr2.end()));
  ASSERT_FALSE(detail::isSameSign(arr3.begin(), arr3.end()));
}

TEST(detail, isOnOneSide)
{
  // Arrange
  auto pl = Plane<double>::getNormalDist({0, 0, 1}, 0);

  Triangle<double> t1{{0, 0, 1}, {1, 0, 1}, {0, 1, 1}};
  Triangle<double> t2{{0, 0, 0}, {1, 0, 1}, {0, 1, 1}};
  Triangle<double> t3{{0, 0, -1}, {1, 0, 1}, {0, 1, 1}};

  // Act & Assert
  ASSERT_TRUE(detail::isOnOneSide(pl, t1));
  ASSERT_FALSE(detail::isOnOneSide(pl, t2));
  ASSERT_FALSE(detail::isOnOneSide(pl, t3));
}

TEST(detail, isCounterClockwise)
{
  // Arrange
  detail::Trian2<double> tr1;
  tr1[0] = {1.0, 0.0}, tr1[1] = {0.0, 1.0}, tr1[2] = {0, -1.0};

  detail::Trian2<double> tr2;
  tr2[0] = {1.0, 0.0}, tr2[1] = {0.0, -1.0}, tr2[2] = {0, 1.0};

  detail::Trian2<double> tr3;
  tr3[0] = {0.0, 1.0}, tr3[1] = {0.0, 0.0}, tr3[2] = {1.0, 0.0};

  detail::Trian2<double> tr4;
  tr4[0] = {0.0, 0.0}, tr4[1] = {0.0, 1.0}, tr4[2] = {1.0, 0.0};

  // Act & Assert
  ASSERT_TRUE(detail::isCounterClockwise(tr1));
  ASSERT_FALSE(detail::isCounterClockwise(tr2));
  ASSERT_TRUE(detail::isCounterClockwise(tr3));
  ASSERT_FALSE(detail::isCounterClockwise(tr4));
}

TEST(detail, isIntersect2D)
{
  // Arrange
  Triangle<double> t1{{1, 0, 0}, {0, 1, 0}, {0, 0, 0}};
  Triangle<double> t2{{3, 0, 0}, {0, 3, 0}, {2, 2, 0}};
  Triangle<double> t3{{3, 0, 0}, {0, 3, 0}, {0, 0, 0}};

  // Act & Assert
  EXPECT_FALSE(detail::isIntersect2D(t1, t2));
  EXPECT_FALSE(detail::isIntersect2D(t2, t1));

  EXPECT_TRUE(detail::isIntersect2D(t1, t3));
  EXPECT_TRUE(detail::isIntersect2D(t3, t1));

  EXPECT_TRUE(detail::isIntersect2D(t2, t3));
  EXPECT_TRUE(detail::isIntersect2D(t3, t2));
}

TEST(detail, getTrian2CounterClockwise)
{
  // Arrange
  auto pl = Plane<double>::getNormalDist({0, 1, 1e-3}, 0);
  Triangle<double> t{{0, 2, 0}, {1, 0, 0}, {0, -1, 1}};
  detail::Trian2<double> res1;
  res1[0] = {0, 0}, res1[1] = {1, 0}, res1[2] = {0, 1};

  // Act
  auto res2 = detail::getTrian2(pl, t);

  // Assert
  EXPECT_EQ(res1[0], res2[0]);
  EXPECT_EQ(res1[1], res2[1]);
  EXPECT_EQ(res1[2], res2[2]);
}

TEST(detail, getTrian2Clockwise)
{
  // Arrange
  auto pl = Plane<double>::getNormalDist({1, 1e-3, 0}, 0);
  Triangle<double> t{{0, 0, 0}, {2, 0, 1}, {-1, 1, 0}};
  detail::Trian2<double> res1;
  res1[0] = {0, 1}, res1[1] = {0, 0}, res1[2] = {1, 0};

  // Act
  auto res2 = detail::getTrian2(pl, t);

  // Assert
  EXPECT_EQ(res1[0], res2[0]);
  EXPECT_EQ(res1[1], res2[1]);
  EXPECT_EQ(res1[2], res2[2]);
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}