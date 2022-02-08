#include <gtest/gtest.h>

#include "geom/vector.hh"

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
  // nothing

  // Assert
  ASSERT_TRUE(v1.isEq(v2));
  ASSERT_TRUE(v2.isEq(v1));

  ASSERT_FALSE(v1.isEq(v3));
  ASSERT_FALSE(v2.isEq(v3));

  ASSERT_FALSE(v3.isEq(v1));
  ASSERT_FALSE(v3.isEq(v2));
}

TEST(Vector, cross)
{
  // Arrange
  geom::Vector<double> v1{1, 2, 3};
  geom::Vector<double> v2{4, 5, 6};

  // Act
  auto res = v1.cross(v2);

  // Assert

  ASSERT_TRUE(res.isEq(geom::Vector<double>{-3, 6, -3}));
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
