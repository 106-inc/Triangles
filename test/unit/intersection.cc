#include <gtest/gtest.h>

#include "intersection/intersection.hh"

using namespace geom;
using FPTypes = testing::Types<float, double, long double>;

//================================================================================================

template <typename T>
class IntersectionTriangles : public testing::Test
{};

TYPED_TEST_SUITE(IntersectionTriangles, FPTypes);

TYPED_TEST(IntersectionTriangles, Parallel1)
{
  // Arrange
  Triangle<TypeParam> t1{{0, 0, 0}, {0, 1, 0}, {0, 0, 1}};
  Triangle<TypeParam> t2{{1, 0, 0}, {1, 1, 0}, {1, 0, 1}};
  Triangle<TypeParam> t3{{1e-4, 0, 0}, {1e-4, 1, 0}, {1e-4, 0, 1}};

  // Act & Assert
  EXPECT_FALSE(isIntersect(t1, t2));
  EXPECT_FALSE(isIntersect(t1, t3));
  EXPECT_FALSE(isIntersect(t2, t3));
}

TYPED_TEST(IntersectionTriangles, Parallel2)
{
  // Arrange
  Triangle<TypeParam> t1{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
  Triangle<TypeParam> t2{{2, 0, 0}, {0, 2, 0}, {0, 0, 2}};

  // Act & Assert
  EXPECT_FALSE(isIntersect(t1, t2));
  EXPECT_FALSE(isIntersect(t2, t1));
}

TYPED_TEST(IntersectionTriangles, SamePlane)
{
  // Arrange
  Triangle<TypeParam> t1{{1, 0, 0}, {0, 1, 0}, {0, 0, 0}};
  Triangle<TypeParam> t2{{3, 0, 0}, {0, 3, 0}, {2, 2, 0}};
  Triangle<TypeParam> t3{{3, 0, 0}, {0, 3, 0}, {0, 0, 0}};

  // Act & Assert
  EXPECT_FALSE(isIntersect(t1, t2));
  EXPECT_FALSE(isIntersect(t2, t1));

  EXPECT_TRUE(isIntersect(t1, t3));
  EXPECT_TRUE(isIntersect(t3, t1));

  EXPECT_TRUE(isIntersect(t2, t3));
  EXPECT_TRUE(isIntersect(t3, t2));
}

TYPED_TEST(IntersectionTriangles, SameSide)
{
  // Arrange
  Triangle<TypeParam> t1{{0, 0, 0}, {0, 1, 0}, {1, 0, 0}};
  Triangle<TypeParam> t2{{0, 0, 1}, {0, 1, 1}, {1, 0, 2}};

  // Act & Assert
  EXPECT_FALSE(isIntersect(t1, t2));
  EXPECT_FALSE(isIntersect(t2, t1));
}

TYPED_TEST(IntersectionTriangles, CommonCase)
{
  // Arrange
  Triangle<TypeParam> t1{{0, 0, 0}, {0, 1, 0}, {1, 0, 0}};
  Triangle<TypeParam> t2{{0, 0, 1}, {0, 1, -1}, {1, 0, -1}};

  // Act & Assert
  EXPECT_TRUE(isIntersect(t1, t2));
}

//================================================================================================

template <typename T>
class IntersectionPlanes : public ::testing::Test
{};

TYPED_TEST_SUITE(IntersectionPlanes, FPTypes);

TYPED_TEST(IntersectionPlanes, Same)
{
  // Arrange
  auto pl1 = Plane<TypeParam>::getNormalDist({0, 0, 1}, 1);
  auto pl2 = Plane<TypeParam>::getNormalDist({0, 0, 1}, 1);

  // Act
  bool res = std::holds_alternative<Plane<TypeParam>>(intersect(pl1, pl2));

  // Assert
  EXPECT_TRUE(res);
}

TYPED_TEST(IntersectionPlanes, Parallel)
{
  // Arrange
  auto pl1 = Plane<TypeParam>::getNormalDist({0, 0, 1}, 1);
  auto pl2 = Plane<TypeParam>::getNormalDist({0, 0, 1}, 2);

  // Act
  bool res = std::holds_alternative<std::monostate>(intersect(pl1, pl2));

  // Assert
  EXPECT_TRUE(res);
}

TYPED_TEST(IntersectionPlanes, Common)
{
  // Arrange
  auto pl1 = Plane<TypeParam>::getBy3Points({0, 0, 0}, {1, 0, 0}, {0, 1, 0});
  auto pl2 = Plane<TypeParam>::getBy3Points({0, 0, 0}, {0, 0, 1}, {0, 1, 0});
  auto pl3 = Plane<TypeParam>::getBy3Points({0, 0, 0}, {0, 0, 1}, {1, 1, 0});

  Line<TypeParam> l1{{0, 0, 0}, {0, 1, 0}};
  Line<TypeParam> l2{{0, 0, 0}, {1, 1, 0}};

  // Act
  auto res12 = std::get<Line<TypeParam>>(intersect(pl1, pl2));
  auto res13 = std::get<Line<TypeParam>>(intersect(pl1, pl3));

  // Assert
  EXPECT_EQ(l1, res12);
  EXPECT_EQ(l2, res13);
}

//================================================================================================

template <typename T>
class IntersectionLines : public ::testing::Test
{};

TYPED_TEST_SUITE(IntersectionLines, FPTypes);

TYPED_TEST(IntersectionLines, Same)
{
  // Arrange
  Line<TypeParam> l1{{1, 1, 0}, {24, 24, 24}};
  Line<TypeParam> l2{{-47, -47, -48}, {24, 24, 24}};

  // Act
  auto res = std::get<Line<TypeParam>>(intersect(l1, l2));
  auto res_rev = std::get<Line<TypeParam>>(intersect(l2, l1));

  // Assert
  EXPECT_EQ(res, l1);
  EXPECT_EQ(res_rev, res);
}

TYPED_TEST(IntersectionLines, Parallel)
{
  // Arrange
  Line<TypeParam> l1{{1, 1, 0}, {24, 24, 24}};
  Line<TypeParam> l2{{0, 0, 0}, {-1, -1, -1}};

  // Act
  bool res = std::holds_alternative<std::monostate>(intersect(l1, l2));
  bool res_rev = std::holds_alternative<std::monostate>(intersect(l2, l1));

  // Assert
  EXPECT_TRUE(res);
  EXPECT_TRUE(res_rev);
}

TYPED_TEST(IntersectionLines, Skew)
{
  // Arrange
  Line<TypeParam> l1{{1, 1, 0}, {24, 24, 24}};
  Line<TypeParam> l2{{0, 0, 0}, {-1, 0, 0}};

  // Act
  bool res = std::holds_alternative<std::monostate>(intersect(l1, l2));
  bool res_rev = std::holds_alternative<std::monostate>(intersect(l2, l1));

  // Assert
  EXPECT_TRUE(res);
  EXPECT_TRUE(res_rev);
}

TYPED_TEST(IntersectionLines, Common)
{
  // Arrange
  Line<TypeParam> l1{{1, 1, 0}, {1, 1, 1}};
  Line<TypeParam> l2{{0, 0, 20}, {0, 0, 1}};

  Line<TypeParam> l3{{1, 0, 0}, {0, -100, 0}};
  Line<TypeParam> l4{{0, 1, 0}, {254, 0, 0}};

  // Act
  auto res12 = std::get<Vec3<TypeParam>>(intersect(l1, l2));
  auto res21 = std::get<Vec3<TypeParam>>(intersect(l2, l1));

  auto res34 = std::get<Vec3<TypeParam>>(intersect(l3, l4));
  auto res43 = std::get<Vec3<TypeParam>>(intersect(l4, l3));

  // Assert
  EXPECT_EQ(res12, Vec3<TypeParam>(0, 0, -1));
  EXPECT_EQ(res21, res12);

  EXPECT_EQ(res34, Vec3<TypeParam>(1, 1, 0));
  EXPECT_EQ(res43, res34);
}

//================================================================================================

// template <typename T>
// class IntersectionDetail : public ::testing::Test
// {};

// TYPED_TEST_SUITE(IntersectionDetail, FPTypes);

TEST(IntersectionDetail, intervalOverlap)
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
  EXPECT_TRUE(detail::isOverlap(interv1, interv2));
  EXPECT_TRUE(detail::isOverlap(interv1, interv3));
  EXPECT_TRUE(detail::isOverlap(interv2, interv1));
  EXPECT_TRUE(detail::isOverlap(interv3, interv1));

  EXPECT_TRUE(detail::isOverlap(interv1, interv4));
  EXPECT_TRUE(detail::isOverlap(interv1, interv5));
  EXPECT_TRUE(detail::isOverlap(interv4, interv1));
  EXPECT_TRUE(detail::isOverlap(interv5, interv1));

  EXPECT_FALSE(detail::isOverlap(interv1, interv6));
  EXPECT_FALSE(detail::isOverlap(interv1, interv7));
  EXPECT_FALSE(detail::isOverlap(interv6, interv1));
  EXPECT_FALSE(detail::isOverlap(interv7, interv1));
}

TEST(IntersectionDetail, isSameSign)
{
  // Arrange
  std::vector<double> arr1{1.0, 34.0, 5.0, 2.0};
  std::vector<double> arr2{-1.0, 34.0, 5.0, 2.0};
  std::vector<double> arr3{0.0, 34.0, 5.0, 2.0};

  // Act & Assert
  EXPECT_TRUE(detail::isSameSign(arr1.begin(), arr1.end()));
  EXPECT_FALSE(detail::isSameSign(arr2.begin(), arr2.end()));
  EXPECT_FALSE(detail::isSameSign(arr3.begin(), arr3.end()));
}

TEST(IntersectionDetail, isOnOneSide)
{
  // Arrange
  auto pl = Plane<double>::getNormalDist({0, 0, 1}, 0);

  Triangle<double> t1{{0, 0, 1}, {1, 0, 1}, {0, 1, 1}};
  Triangle<double> t2{{0, 0, 0}, {1, 0, 1}, {0, 1, 1}};
  Triangle<double> t3{{0, 0, -1}, {1, 0, 1}, {0, 1, 1}};

  // Act & Assert
  EXPECT_TRUE(detail::isOnOneSide(pl, t1));
  EXPECT_FALSE(detail::isOnOneSide(pl, t2));
  EXPECT_FALSE(detail::isOnOneSide(pl, t3));
}

TEST(IntersectionDetail, isCounterClockwise)
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
  EXPECT_TRUE(detail::isCounterClockwise(tr1));
  EXPECT_FALSE(detail::isCounterClockwise(tr2));
  EXPECT_TRUE(detail::isCounterClockwise(tr3));
  EXPECT_FALSE(detail::isCounterClockwise(tr4));
}

TEST(IntersectionDetail, isIntersect2D)
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

TEST(IntersectionDetail, getTrian2CounterClockwise)
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

TEST(IntersectionDetail, getTrian2Clockwise)
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

TEST(IntersectionDetail, isIntersectPointTriangle)
{
  // Arrange
  Triangle<double> tr{{0, 0, 0}, {1, 0, 0}, {0, 1, 0}};

  // Act & Assert
  EXPECT_TRUE(detail::isIntersectPointTriangle({0, 0, 0}, tr));
  EXPECT_TRUE(detail::isIntersectPointTriangle({1, 0, 0}, tr));
  EXPECT_TRUE(detail::isIntersectPointTriangle({0, 1, 0}, tr));
  EXPECT_TRUE(detail::isIntersectPointTriangle({0.5, 0.5, 0}, tr));
  EXPECT_TRUE(detail::isIntersectPointTriangle({0.25, 0.25, 0}, tr));

  EXPECT_FALSE(detail::isIntersectPointTriangle({2, 0, 0}, tr));
  EXPECT_FALSE(detail::isIntersectPointTriangle({0, 2, 0}, tr));
  EXPECT_FALSE(detail::isIntersectPointTriangle({1, 1, 0}, tr));
  EXPECT_FALSE(detail::isIntersectPointTriangle({-1, 0, 0}, tr));
  EXPECT_FALSE(detail::isIntersectPointTriangle({0, -1, 0}, tr));
  EXPECT_FALSE(detail::isIntersectPointTriangle({-1, -1, 0}, tr));
}

TEST(IntersectionDetail, isIntersectValidInvalid_Segment1)
{
  // Arrange
  Triangle<double> tr{{0, 0, 0}, {1, 0, 0}, {0, 1, 0}};

  // Act & Assert
  EXPECT_TRUE(detail::isIntersectValidInvalid(tr, {{0, 0, 0}, {0, 0, 0}, {0, 0, -1}}));
  EXPECT_TRUE(detail::isIntersectValidInvalid(tr, {{0, 0, 0}, {0, 0, 1}, {0, 0, -1}}));
  EXPECT_TRUE(detail::isIntersectValidInvalid(tr, {{0, 0, 1}, {0, 0, 1}, {0, 0, -1}}));

  EXPECT_TRUE(detail::isIntersectValidInvalid(tr, {{1, 0, 0}, {1, 0, 0}, {1, 0, -1}}));
  EXPECT_TRUE(detail::isIntersectValidInvalid(tr, {{1, 0, 0}, {1, 0, 1}, {1, 0, -1}}));
  EXPECT_TRUE(detail::isIntersectValidInvalid(tr, {{1, 0, 1}, {1, 0, 1}, {1, 0, -1}}));

  EXPECT_TRUE(detail::isIntersectValidInvalid(tr, {{0, 1, 0}, {0, 1, 0}, {0, 1, -1}}));
  EXPECT_TRUE(detail::isIntersectValidInvalid(tr, {{0, 1, 0}, {0, 1, 1}, {0, 1, -1}}));
  EXPECT_TRUE(detail::isIntersectValidInvalid(tr, {{0, 1, 1}, {0, 1, 1}, {0, 1, -1}}));

  EXPECT_TRUE(detail::isIntersectValidInvalid(tr, {{0.5, 0.5, 0}, {0.5, 0.5, 0}, {0.5, 0.5, -1}}));
  EXPECT_TRUE(detail::isIntersectValidInvalid(tr, {{0.5, 0.5, 0}, {0.5, 0.5, 1}, {0.5, 0.5, -1}}));
  EXPECT_TRUE(detail::isIntersectValidInvalid(tr, {{0.5, 0.5, 1}, {0.5, 0.5, 1}, {0.5, 0.5, -1}}));

  EXPECT_TRUE(
    detail::isIntersectValidInvalid(tr, {{0.25, 0.25, 0}, {0.25, 0.25, 0}, {0.25, 0.25, -1}}));
  EXPECT_TRUE(
    detail::isIntersectValidInvalid(tr, {{0.25, 0.25, 0}, {0.25, 0.25, 1}, {0.25, 0.25, -1}}));
  EXPECT_TRUE(
    detail::isIntersectValidInvalid(tr, {{0.25, 0.25, 1}, {0.25, 0.25, 1}, {0.25, 0.25, -1}}));
}

TEST(IntersectionDetail, isIntersectValidInvalid_Segment2)
{
  // Arrange
  Triangle<double> tr{{0, 0, 0}, {1, 0, 0}, {0, 1, 0}};

  // Act & Assert
  EXPECT_FALSE(detail::isIntersectValidInvalid(tr, {{5, 5, 0}, {5, 5, 0}, {5, 5, -1}}));
  EXPECT_FALSE(detail::isIntersectValidInvalid(tr, {{5, 5, 0}, {5, 5, 1}, {5, 5, -1}}));
  EXPECT_FALSE(detail::isIntersectValidInvalid(tr, {{5, 5, 1}, {5, 5, 1}, {5, 5, -1}}));

  EXPECT_FALSE(detail::isIntersectValidInvalid(tr, {{2, 0, 0}, {2, 0, 0}, {2, 0, -1}}));
  EXPECT_FALSE(detail::isIntersectValidInvalid(tr, {{2, 0, 0}, {2, 0, 1}, {2, 0, -1}}));
  EXPECT_FALSE(detail::isIntersectValidInvalid(tr, {{2, 0, 1}, {2, 0, 1}, {2, 0, -1}}));

  EXPECT_FALSE(detail::isIntersectValidInvalid(tr, {{0, 2, 0}, {0, 2, 0}, {0, 2, -1}}));
  EXPECT_FALSE(detail::isIntersectValidInvalid(tr, {{0, 2, 0}, {0, 2, 1}, {0, 2, -1}}));
  EXPECT_FALSE(detail::isIntersectValidInvalid(tr, {{0, 2, 1}, {0, 2, 1}, {0, 2, -1}}));

  EXPECT_FALSE(detail::isIntersectValidInvalid(tr, {{-1, 0, 0}, {-1, 0, 0}, {-1, 0, -1}}));
  EXPECT_FALSE(detail::isIntersectValidInvalid(tr, {{-1, 0, 0}, {-1, 0, 1}, {-1, 0, -1}}));
  EXPECT_FALSE(detail::isIntersectValidInvalid(tr, {{-1, 0, 1}, {-1, 0, 1}, {-1, 0, -1}}));

  EXPECT_FALSE(detail::isIntersectValidInvalid(tr, {{0, -1, 0}, {0, -1, 0}, {0, -1, -1}}));
  EXPECT_FALSE(detail::isIntersectValidInvalid(tr, {{0, -1, 0}, {0, -1, 1}, {0, -1, -1}}));
  EXPECT_FALSE(detail::isIntersectValidInvalid(tr, {{0, -1, 1}, {0, -1, 1}, {0, -1, -1}}));
}

TEST(IntersectionDetail, isIntersectPointSegment)
{
  // Arrange
  detail::Segment3D<double> segm{{1, 0, 1}, {0, 1, 1}};

  // Act & Assert
  EXPECT_FALSE(detail::isIntersectPointSegment({-1, 2, 1}, segm));
  EXPECT_FALSE(detail::isIntersectPointSegment({2, -1, 1}, segm));
  EXPECT_FALSE(detail::isIntersectPointSegment({0, 0, 0}, segm));

  EXPECT_TRUE(detail::isIntersectPointSegment({0.5, 0.5, 1}, segm));
  EXPECT_TRUE(detail::isIntersectPointSegment({1, 0, 1}, segm));
  EXPECT_TRUE(detail::isIntersectPointSegment({0, 1, 1}, segm));
}

TEST(IntersectionDetail, isIntersectSegmentSegment1)
{
  // Arrange
  detail::Segment3D<double> segm1{{-1, 0, 0}, {1, 0, 0}};
  detail::Segment3D<double> segm2{{1, 0, 0}, {-1, 0, 0}};

  detail::Segment3D<double> segm3{{0, 0, 0}, {2, 0, 0}};
  detail::Segment3D<double> segm4{{-2, 0, 0}, {0, 0, 0}};

  detail::Segment3D<double> segm5{{-2, 0, 0}, {2, 0, 0}};
  detail::Segment3D<double> segm6{{-0.5, 0, 0}, {0.5, 0, 0}};

  detail::Segment3D<double> segm7{{1, 0, 0}, {2, 0, 0}};
  detail::Segment3D<double> segm8{{-2, 0, 0}, {-1, 0, 0}};

  detail::Segment3D<double> segm9{{2, 0, 0}, {3, 0, 0}};
  detail::Segment3D<double> segm10{{-3, 0, 0}, {-2, 0, 0}};

  // Act & Assert
  EXPECT_TRUE(detail::isIntersectSegmentSegment(segm1, segm1));

  EXPECT_TRUE(detail::isIntersectSegmentSegment(segm1, segm2));
  EXPECT_TRUE(detail::isIntersectSegmentSegment(segm2, segm1));

  EXPECT_TRUE(detail::isIntersectSegmentSegment(segm1, segm3));
  EXPECT_TRUE(detail::isIntersectSegmentSegment(segm3, segm1));

  EXPECT_TRUE(detail::isIntersectSegmentSegment(segm1, segm4));
  EXPECT_TRUE(detail::isIntersectSegmentSegment(segm4, segm1));

  EXPECT_TRUE(detail::isIntersectSegmentSegment(segm1, segm5));
  EXPECT_TRUE(detail::isIntersectSegmentSegment(segm5, segm1));

  EXPECT_TRUE(detail::isIntersectSegmentSegment(segm1, segm6));
  EXPECT_TRUE(detail::isIntersectSegmentSegment(segm6, segm1));

  EXPECT_TRUE(detail::isIntersectSegmentSegment(segm1, segm7));
  EXPECT_TRUE(detail::isIntersectSegmentSegment(segm7, segm1));

  EXPECT_TRUE(detail::isIntersectSegmentSegment(segm1, segm8));
  EXPECT_TRUE(detail::isIntersectSegmentSegment(segm8, segm1));

  EXPECT_FALSE(detail::isIntersectSegmentSegment(segm1, segm9));
  EXPECT_FALSE(detail::isIntersectSegmentSegment(segm9, segm1));

  EXPECT_FALSE(detail::isIntersectSegmentSegment(segm1, segm10));
  EXPECT_FALSE(detail::isIntersectSegmentSegment(segm10, segm1));
}

TEST(IntersectionDetail, isIntersectSegmentSegment2)
{
  // Arrange
  detail::Segment3D<double> segm1{{-1, 0, 0}, {1, 0, 0}};
  detail::Segment3D<double> segm2{{0, -1, 0}, {0, 1, 0}};
  detail::Segment3D<double> segm3{{0, -1, 0}, {0, 0, 0}};
  detail::Segment3D<double> segm4{{0, 0, 0}, {0, 1, 0}};
  detail::Segment3D<double> segm5{{0, 1, 0}, {0, 2, 0}};
  detail::Segment3D<double> segm6{{0, -2, 0}, {0, -1, 0}};
  detail::Segment3D<double> segm7{{-1, 2, 0}, {1, 2, 0}};

  // Act & Assert
  EXPECT_TRUE(detail::isIntersectSegmentSegment(segm1, segm2));
  EXPECT_TRUE(detail::isIntersectSegmentSegment(segm2, segm1));

  EXPECT_TRUE(detail::isIntersectSegmentSegment(segm1, segm3));
  EXPECT_TRUE(detail::isIntersectSegmentSegment(segm3, segm1));

  EXPECT_TRUE(detail::isIntersectSegmentSegment(segm1, segm4));
  EXPECT_TRUE(detail::isIntersectSegmentSegment(segm4, segm1));

  EXPECT_FALSE(detail::isIntersectSegmentSegment(segm1, segm5));
  EXPECT_FALSE(detail::isIntersectSegmentSegment(segm5, segm1));

  EXPECT_FALSE(detail::isIntersectSegmentSegment(segm1, segm6));
  EXPECT_FALSE(detail::isIntersectSegmentSegment(segm6, segm1));

  EXPECT_FALSE(detail::isIntersectSegmentSegment(segm1, segm7));
  EXPECT_FALSE(detail::isIntersectSegmentSegment(segm7, segm1));
}

TEST(IntersectionDetail, isIntersectIntersectionTrianglesegment2D)
{
  // Arrange
  Triangle<double> tr{{1, 0, 0}, {0, 1, 0}, {0, 0, 0}};
  // Act & Assert
  EXPECT_TRUE(detail::isIntersect2D<double>(tr, {{-1, -1, -1}, {1, 1, 1}, {0, 0, 0}}));
  EXPECT_TRUE(detail::isIntersect2D<double>(tr, {{-1, -1, -1}, {0, 0, 0}, {0, 0, 0}}));
  EXPECT_TRUE(detail::isIntersect2D<double>(tr, {{1, 1, 1}, {0, 0, 0}, {0, 0, 0}}));

  EXPECT_FALSE(detail::isIntersect2D<double>(tr, {{1, 1, 1}, {2, 2, 2}, {2, 2, 2}}));
  EXPECT_FALSE(detail::isIntersect2D<double>(tr, {{-1, -1, -1}, {-2, -2, -2}, {-2, -2, -2}}));

  EXPECT_TRUE(detail::isIntersect2D<double>(tr, {{1, 0, 0}, {2, 0, 0}, {2, 0, 0}}));
  EXPECT_TRUE(detail::isIntersect2D<double>(tr, {{-1, 0, 0}, {0, 0, 0}, {0, 0, 0}}));

  EXPECT_FALSE(detail::isIntersect2D<double>(tr, {{2, 0, 0}, {3, 0, 0}, {2.5, 0, 0}}));
  EXPECT_FALSE(detail::isIntersect2D<double>(tr, {{-2, 0, 0}, {-1, 0, 0}, {-2, 0, 0}}));

  EXPECT_TRUE(detail::isIntersect2D<double>(tr, {{0, 1, 0}, {0, 2, 0}, {0, 2, 0}}));
  EXPECT_TRUE(detail::isIntersect2D<double>(tr, {{0, -1, 0}, {0, 0, 0}, {0, 0, 0}}));

  EXPECT_FALSE(detail::isIntersect2D<double>(tr, {{0, 2, 0}, {0, 3, 0}, {0, 2.5, 0}}));
  EXPECT_FALSE(detail::isIntersect2D<double>(tr, {{0, -2, 0}, {0, -1, 0}, {0, -2, 0}}));
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
