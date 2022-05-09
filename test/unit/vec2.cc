#include "primitives/vec2.hh"
#include "test_header.hh"

using namespace geom;

template <typename T>
class Vec2Test : public testing::Test
{};

TYPED_TEST_SUITE(Vec2Test, FPTypes);

TYPED_TEST(Vec2Test, copyCtor)
{
  // Arrange
  Vec2<TypeParam> v1{1, 2};
  Vec2<TypeParam> v2 = v1;
  Vec2<TypeParam> v3{v1};

  // Act & Arrange
  EXPECT_EQ(v1, v2);
  EXPECT_EQ(v1, v3);
}

TYPED_TEST(Vec2Test, dot)
{
  // Arrange
  Vec2<TypeParam> v1{1, 2};
  Vec2<TypeParam> v2{4, 5};

  // Act
  auto res = v1.dot(v2);

  // Assert
  EXPECT_NEAR(res, 14, ThresComp<TypeParam>::getThreshold());
}

TYPED_TEST(Vec2Test, isEq)
{
  // Arrange
  Vec2<TypeParam> v1{1.0, 2.0};
  Vec2<TypeParam> v2{1.0, 2.0};
  Vec2<TypeParam> v3{4.0, 5.0};

  // Act
  /* nothing */

  // Assert
  EXPECT_TRUE(v1.isEqual(v2));
  EXPECT_TRUE(v2.isEqual(v1));

  EXPECT_FALSE(v1.isEqual(v3));
  EXPECT_FALSE(v2.isEqual(v3));

  EXPECT_FALSE(v3.isEqual(v1));
  EXPECT_FALSE(v3.isEqual(v2));
}

TYPED_TEST(Vec2Test, normalize)
{
  // Arrange
  Vec2<TypeParam> v1{3, 4};
  Vec2<TypeParam> v2{0, 0};
  Vec2<TypeParam> v3{0, 1};

  // Act
  auto res1 = v1.normalized();
  auto res2 = v2.normalized();
  auto res3 = v3.normalized();

  // Assert
  EXPECT_TRUE(res1.isEqual(v1 / 5));
  EXPECT_TRUE(res2.isEqual(v2));
  EXPECT_TRUE(res3.isEqual(v3));
}

TYPED_TEST(Vec2Test, isPar)
{
  // Arrange
  Vec2<TypeParam> v1{2, -6};
  Vec2<TypeParam> v2{8, -24};
  Vec2<TypeParam> v3{6, 4};
  Vec2<TypeParam> vz{0};

  // Act & Assert
  EXPECT_TRUE(v1.isPar(v2));
  EXPECT_FALSE(v1.isPar(v3));
  EXPECT_FALSE(v3.isPar(v2));

  EXPECT_TRUE(v1.isPar(vz));
  EXPECT_TRUE(v2.isPar(vz));
  EXPECT_TRUE(v3.isPar(vz));
}

TYPED_TEST(Vec2Test, isPerp)
{
  // Arrange
  Vec2<TypeParam> v1{1, 1};
  Vec2<TypeParam> v2{-1, 1};
  Vec2<TypeParam> v3{1, 3};
  Vec2<TypeParam> vz{0};

  // Act & Assert
  EXPECT_TRUE(v1.isPerp(v2));
  EXPECT_TRUE(v2.isPerp(v1));

  EXPECT_FALSE(v1.isPerp(v3));
  EXPECT_FALSE(v3.isPerp(v1));
  EXPECT_FALSE(v2.isPerp(v3));
  EXPECT_FALSE(v3.isPerp(v2));

  EXPECT_TRUE(v1.isPerp(vz));
  EXPECT_TRUE(v2.isPerp(vz));
  EXPECT_TRUE(v3.isPerp(vz));
}

TYPED_TEST(Vec2Test, getPerp)
{
  // Arrange
  Vec2<TypeParam> v1{34, 54};

  // Act
  auto v2 = v1.getPerp();
  auto v3 = v2.getPerp();

  // Assert
  EXPECT_TRUE(v1.isPerp(v2));
  EXPECT_TRUE(v2.isPerp(v1));

  EXPECT_TRUE(v3.isPerp(v2));
  EXPECT_TRUE(v2.isPerp(v3));

  EXPECT_TRUE(v1.isPar(v3));
  EXPECT_TRUE(v3.isPar(v1));
}

#include "test_footer.hh"
