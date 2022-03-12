#include <gtest/gtest.h>

#include "primitives/primitives.hh"

using namespace geom;

TEST(Vec2, copyCtor)
{
  // Arrange
  Vec2D v1{1, 2};
  Vec2D v2 = v1;
  Vec2D v3{v1};

  // Act & Arrange
  ASSERT_EQ(v1, v2);
  ASSERT_EQ(v1, v3);
}

TEST(Vec2, dot)
{
  // Arrange
  Vec2D v1{1, 2};
  Vec2D v2{4, 5};

  // Act
  auto res = v1.dot(v2);

  // Assert
  ASSERT_DOUBLE_EQ(res, 14);
}

TEST(Vec2, isEq)
{
  // Arrange
  Vec2D v1{1.0, 2.0};
  Vec2D v2{1.0, 2.0};
  Vec2D v3{4.0, 5.0};

  // Act
  /* nothing */

  // Assert
  ASSERT_TRUE(v1.isEqual(v2));
  ASSERT_TRUE(v2.isEqual(v1));

  ASSERT_FALSE(v1.isEqual(v3));
  ASSERT_FALSE(v2.isEqual(v3));

  ASSERT_FALSE(v3.isEqual(v1));
  ASSERT_FALSE(v3.isEqual(v2));
}

TEST(Vec2, normalize)
{
  // Arrange
  Vec2F v1{3, 4};
  Vec2F v2{0, 0};
  Vec2F v3{0, 1};

  // Act
  auto res1 = v1.normalized();
  auto res2 = v2.normalized();
  auto res3 = v3.normalized();

  // Assert
  ASSERT_TRUE(res1.isEqual(v1 / 5));
  ASSERT_TRUE(res2.isEqual(v2));
  ASSERT_TRUE(res3.isEqual(v3));
}

TEST(Vec2, isPar)
{
  // Arrange
  Vec2F v1{2, -6};
  Vec2F v2{8, -24};
  Vec2F v3{6, 4};
  Vec2F vz{0};

  // Act & Assert
  ASSERT_TRUE(v1.isPar(v2));
  ASSERT_FALSE(v1.isPar(v3));
  ASSERT_FALSE(v3.isPar(v2));

  ASSERT_TRUE(v1.isPar(vz));
  ASSERT_TRUE(v2.isPar(vz));
  ASSERT_TRUE(v3.isPar(vz));
}

TEST(Vec2, isPerp)
{
  // Arrange
  Vec2F v1{1, 1};
  Vec2F v2{-1, 1};
  Vec2F v3{1, 3};
  Vec2F vz{0};

  // Act & Assert
  ASSERT_TRUE(v1.isPerp(v2));
  ASSERT_TRUE(v2.isPerp(v1));

  ASSERT_FALSE(v1.isPerp(v3));
  ASSERT_FALSE(v3.isPerp(v1));
  ASSERT_FALSE(v2.isPerp(v3));
  ASSERT_FALSE(v3.isPerp(v2));

  ASSERT_TRUE(v1.isPerp(vz));
  ASSERT_TRUE(v2.isPerp(vz));
  ASSERT_TRUE(v3.isPerp(vz));
}

TEST(Vec2, getPerp)
{
  // Arrange
  Vec2D v1{34, 54};

  // Act
  auto v2 = v1.getPerp();
  auto v3 = v2.getPerp();

  // Assert
  ASSERT_TRUE(v1.isPerp(v2));
  ASSERT_TRUE(v2.isPerp(v1));

  ASSERT_TRUE(v3.isPerp(v2));
  ASSERT_TRUE(v2.isPerp(v3));

  ASSERT_TRUE(v1.isPar(v3));
  ASSERT_TRUE(v3.isPar(v1));
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
