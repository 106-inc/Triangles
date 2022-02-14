#include <gtest/gtest.h>

#include "primitives/vector.hh"

TEST(Vector, dot)
{
  // Arrange
  geom::Vector<double> v1{1, 2, 3};
  geom::Vector<double> v2{4, 5, 6};

  // Act
  auto res = v1.dot(v2);

  // Assert
  ASSERT_DOUBLE_EQ(res, 32);
}

TEST(Vector, isEq)
{
  // Arrange
  geom::Vector<double> v1{1.0, 2.0, 3.0};
  geom::Vector<double> v2{1.0, 2.0, 3.0};
  geom::Vector<double> v3{4.0, 5.0, 5.0};

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

TEST(Vector, cross)
{
  // Arrange
  geom::Vector<double> v1{1, 2, 3};
  geom::Vector<double> v2{4, 5, 6};

  // Act
  auto res = v1.cross(v2);

  // Assert
  ASSERT_TRUE(res.isEqual(geom::Vector<double>{-3, 6, -3}));
}

TEST(Vector, normalize)
{
  // Arrange
  geom::Vector<float> v1{2, 6, 9};
  geom::Vector<float> v2{0, 0, 0};
  geom::Vector<float> v3{0, 0, 1};

  // Act
  auto res1 = v1.normalized();
  auto res2 = v2.normalized();
  auto res3 = v3.normalized();

  // Assert
  ASSERT_TRUE(res1.isEqual(v1 / 11));
  ASSERT_TRUE(res2.isEqual(v2));
  ASSERT_TRUE(res3.isEqual(v3));
}

TEST(Vector, isParallel)
{
  // Arrange
  geom::VectorF v1{2, -6, 9};
  geom::VectorF v2{8, -24, 36};
  geom::VectorF v3{6, 4, 1};

  // Act
  auto res12 = v1.isParallel(v2);
  auto res13 = v1.isParallel(v3);
  auto res23 = v3.isParallel(v2);

  // Assert
  ASSERT_TRUE(res12);
  ASSERT_FALSE(res13);
  ASSERT_FALSE(res23);
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
