#include <gtest/gtest.h>

#include "primitives/primitives.hh"

TEST(Vec3, copyCtor)
{
  // Arrange
  geom::Vec3<double> v1{1, 2, 3};
  geom::Vec3<double> v2 = v1;
  geom::Vec3<double> v3{v1};

  // Act & Arrange
  ASSERT_EQ(v1, v2);
  ASSERT_EQ(v1, v3);
}

TEST(Vec3, dot)
{
  // Arrange
  geom::Vec3<double> v1{1, 2, 3};
  geom::Vec3<double> v2{4, 5, 6};

  // Act
  auto res = v1.dot(v2);

  // Assert
  ASSERT_DOUBLE_EQ(res, 32);
}

TEST(Vec3, isEq)
{
  // Arrange
  geom::Vec3<double> v1{1.0, 2.0, 3.0};
  geom::Vec3<double> v2{1.0, 2.0, 3.0};
  geom::Vec3<double> v3{4.0, 5.0, 5.0};

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

TEST(Vec3, cross)
{
  // Arrange
  geom::Vec3<double> v1{1, 2, 3};
  geom::Vec3<double> v2{4, 5, 6};

  // Act
  auto res = v1.cross(v2);

  // Assert
  ASSERT_TRUE(res.isEqual(geom::Vec3<double>{-3, 6, -3}));
}

TEST(Vec3, normalize)
{
  // Arrange
  geom::Vec3<float> v1{2, 6, 9};
  geom::Vec3<float> v2{0, 0, 0};
  geom::Vec3<float> v3{0, 0, 1};

  // Act
  auto res1 = v1.normalized();
  auto res2 = v2.normalized();
  auto res3 = v3.normalized();

  // Assert
  ASSERT_TRUE(res1.isEqual(v1 / 11));
  ASSERT_TRUE(res2.isEqual(v2));
  ASSERT_TRUE(res3.isEqual(v3));
}

TEST(Vec3, isPar)
{
  // Arrange
  geom::Vec3F v1{2, -6, 9};
  geom::Vec3F v2{8, -24, 36};
  geom::Vec3F v3{6, 4, 1};

  // Act
  auto res12 = v1.isPar(v2);
  auto res13 = v1.isPar(v3);
  auto res23 = v3.isPar(v2);

  // Assert
  ASSERT_TRUE(res12);
  ASSERT_FALSE(res13);
  ASSERT_FALSE(res23);
}

TEST(Vec3, isPerp)
{
  // Arrange
  geom::Vec3F v1{1, 1, 0};
  geom::Vec3F v2{-1, 1, 0};
  geom::Vec3F v3{0, 0, 1};
  geom::Vec3F v4{1, 1, 1};
  geom::Vec3F vz{0};

  // Act

  // Assert
  ASSERT_TRUE(v1.isPerp(v2));
  ASSERT_TRUE(v2.isPerp(v1));

  ASSERT_TRUE(v1.isPerp(v3));
  ASSERT_TRUE(v2.isPerp(v3));

  ASSERT_TRUE(v3.isPerp(v1));
  ASSERT_TRUE(v3.isPerp(v2));

  ASSERT_TRUE(v2.isPerp(v4));
  ASSERT_TRUE(v4.isPerp(v2));

  ASSERT_TRUE(vz.isPerp(v1));

  ASSERT_FALSE(v1.isPerp(v4));
  ASSERT_FALSE(v3.isPerp(v4));

  ASSERT_FALSE(v4.isPerp(v1));
  ASSERT_FALSE(v4.isPerp(v3));

  ASSERT_FALSE(v4.isPerp(v4));
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
