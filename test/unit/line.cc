#include <gtest/gtest.h>

#include "primitives/primitives.hh"

using namespace geom;
using FPTypes = testing::Types<float, double, long double>;

template <typename T>
class LineTest : public testing::Test
{};

TYPED_TEST_SUITE(LineTest, FPTypes);

TYPED_TEST(LineTest, copyCtor)
{
  // Arrange
  Line<TypeParam> l1{{1, 1, 1}, {2, 2, 2}};
  auto l2{l1};
  auto l3 = l1;

  // Act & Assert
  EXPECT_EQ(l1, l2);
  EXPECT_EQ(l1, l3);
}

TYPED_TEST(LineTest, getset)
{
  // Arrange
  Vec3<TypeParam> v1{1, 0, 0};
  Vec3<TypeParam> v2{0, 1, 0};
  Line<TypeParam> l{v1, v2};

  // Act
  auto org = l.org();
  auto dir = l.dir();

  // Assert
  EXPECT_EQ(org, v1);
  EXPECT_EQ(dir, v2);
}

TYPED_TEST(LineTest, getPoint)
{
  // Arrange
  Vec3<TypeParam> v1{1, 0, 0};
  Vec3<TypeParam> v2{0, 1, 0};
  Line<TypeParam> l1{v1, v2};
  Line<TypeParam> l2{{456, 30, -68}, {-456, -30, 68}};

  // Act
  auto point = l1.getPoint(20);
  auto point2 = l1.getPoint(0);
  auto point3 = l2.getPoint(1);

  // Assert
  EXPECT_EQ(point, Vec3<TypeParam>(1, 20, 0));
  EXPECT_EQ(point2, l1.org());

  EXPECT_EQ(point3, Vec3<TypeParam>());
}

TYPED_TEST(LineTest, belongs)
{
  // Arrange
  Vec3<TypeParam> v1{1, 1, 2};
  Vec3<TypeParam> v2{1, 1, 3};
  Line<TypeParam> l{{0, 0, 1}, {1, 1, 1}};

  // Act
  bool res1 = l.belongs(v1);
  bool res2 = l.belongs(v2);

  // Assert
  EXPECT_TRUE(res1);
  EXPECT_FALSE(res2);
}

TYPED_TEST(LineTest, isEqual)
{
  // Arrange
  Line<TypeParam> l1{{0, 0, 1}, {1, 1, 1}};
  Line<TypeParam> l2{{1, 1, 2}, {6, 6, 6}};
  Line<TypeParam> l3{{-1, 1, 2}, {6, 6, 6}};

  // Act & Assert
  EXPECT_TRUE(l1.isEqual(l2));
  EXPECT_TRUE(l2.isEqual(l1));

  EXPECT_EQ(l1, l2);
  EXPECT_EQ(l2, l1);

  EXPECT_FALSE(l1.isEqual(l3));
  EXPECT_FALSE(l2.isEqual(l3));

  EXPECT_FALSE(l3.isEqual(l1));
  EXPECT_FALSE(l3.isEqual(l2));

  EXPECT_NE(l1, l3);
  EXPECT_NE(l2, l3);

  EXPECT_NE(l3, l1);
  EXPECT_NE(l3, l2);

  EXPECT_EQ(l1, l1);
  EXPECT_EQ(l2, l2);
  EXPECT_EQ(l3, l3);
}

TYPED_TEST(LineTest, isPar)
{
  // Arrange
  Line<TypeParam> l1{{0, 0, 1}, {1, 1, 1}};
  Line<TypeParam> l2{{5, 3, 2}, {-1, -1, -1}};
  Line<TypeParam> l3{{0, 0, 0}, {1, 0, 0}};

  // Act & Assert
  EXPECT_TRUE(l1.isPar(l2));
  EXPECT_TRUE(l2.isPar(l1));

  EXPECT_FALSE(l2.isPar(l3));
  EXPECT_FALSE(l1.isPar(l3));
}

TYPED_TEST(LineTest, isSkew)
{
  // Arrange
  Line<TypeParam> l1{{0, 0, 1}, {1, 1, 1}};
  Line<TypeParam> l2{{253, 253, 254}, {-6, -6, -6}};
  Line<TypeParam> l3{{5, 3, 2}, {-3, -1, 1}};
  Line<TypeParam> l4{{0, 0, 0}, {1, 0, 0}};

  // Act & Assert
  EXPECT_FALSE(l1.isSkew(l2));
  EXPECT_FALSE(l2.isSkew(l1));

  EXPECT_FALSE(l1.isSkew(l3));
  EXPECT_FALSE(l3.isSkew(l1));

  EXPECT_TRUE(l1.isSkew(l4));
  EXPECT_TRUE(l4.isSkew(l1));

  EXPECT_TRUE(l2.isSkew(l4));
  EXPECT_TRUE(l4.isSkew(l2));
}

TYPED_TEST(LineTest, getBy2Points)
{
  // Arrange
  Vec3<TypeParam> p1{1, 0, 0};
  Vec3<TypeParam> p2{0, 1, 0};

  // Act
  auto l = Line<TypeParam>::getBy2Points(p1, p2);

  // Assert
  EXPECT_TRUE(l.belongs(p1));
  EXPECT_TRUE(l.belongs(p2));

  EXPECT_TRUE(l.dir().isPar(p1 - p2));
  EXPECT_TRUE(l.dir().isPar(p2 - p1));
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
