#include <gtest/gtest.h>

#include "primitives/primitives.hh"

TEST(Vec3, copyCtor)
{
  // Arrange
  geom::Vec3<double> v1{1, 2, 3};
  geom::Vec3<double> v2 = v1;
  geom::Vec3<double> v3{v1};

  // Act & Arrange
  EXPECT_EQ(v1, v2);
  EXPECT_EQ(v1, v3);
}

TEST(Vec3, dot)
{
  // Arrange
  geom::Vec3<double> v1{1, 2, 3};
  geom::Vec3<double> v2{4, 5, 6};

  // Act
  auto res = v1.dot(v2);

  // Assert
  EXPECT_DOUBLE_EQ(res, 32);
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
  EXPECT_TRUE(v1.isEqual(v2));
  EXPECT_TRUE(v2.isEqual(v1));

  EXPECT_FALSE(v1.isEqual(v3));
  EXPECT_FALSE(v2.isEqual(v3));

  EXPECT_FALSE(v3.isEqual(v1));
  EXPECT_FALSE(v3.isEqual(v2));
}

TEST(Vec3, cross)
{
  // Arrange
  geom::Vec3<double> v1{1, 2, 3};
  geom::Vec3<double> v2{4, 5, 6};

  // Act
  auto res = v1.cross(v2);

  // Assert
  EXPECT_TRUE(res.isEqual(geom::Vec3<double>{-3, 6, -3}));
}

TEST(Vec3, triple)
{
  // Arrange
  geom::Vec3<double> v1{1, 2, 3};
  geom::Vec3<double> v2{4, 5, 6};
  geom::Vec3<double> v3{7, 8, 9};

  geom::Vec3<double> v4{1, 0, 0};
  geom::Vec3<double> v5{0, 1, 0};
  geom::Vec3<double> v6{0, 0, 1};

  // Act
  auto res = geom::triple(v1, v2, v3);
  auto nres = geom::triple(v2, v1, v3);

  auto res2 = geom::triple(v4, v5, v6);
  auto nres2 = geom::triple(v6, v5, v4);

  // Assert
  EXPECT_TRUE(geom::Vec3<double>::isNumEq(res, 0));
  EXPECT_TRUE(geom::Vec3<double>::isNumEq(nres, 0));

  EXPECT_TRUE(geom::Vec3<double>::isNumEq(res2, 1));
  EXPECT_TRUE(geom::Vec3<double>::isNumEq(nres2, -1));
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
  EXPECT_TRUE(res1.isEqual(v1 / 11));
  EXPECT_TRUE(res2.isEqual(v2));
  EXPECT_TRUE(res3.isEqual(v3));
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
  EXPECT_TRUE(res12);
  EXPECT_FALSE(res13);
  EXPECT_FALSE(res23);
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
  EXPECT_TRUE(v1.isPerp(v2));
  EXPECT_TRUE(v2.isPerp(v1));

  EXPECT_TRUE(v1.isPerp(v3));
  EXPECT_TRUE(v2.isPerp(v3));

  EXPECT_TRUE(v3.isPerp(v1));
  EXPECT_TRUE(v3.isPerp(v2));

  EXPECT_TRUE(v2.isPerp(v4));
  EXPECT_TRUE(v4.isPerp(v2));

  EXPECT_TRUE(vz.isPerp(v1));

  EXPECT_FALSE(v1.isPerp(v4));
  EXPECT_FALSE(v3.isPerp(v4));

  EXPECT_FALSE(v4.isPerp(v1));
  EXPECT_FALSE(v4.isPerp(v3));

  EXPECT_FALSE(v4.isPerp(v4));
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}