#include <sstream>

#include <gtest/gtest.h>

#include "primitives/primitives.hh"

using namespace geom;

TEST(Triangle, ctor)
{
  // Arrange
  Vec3D v1{1, 2, 3};
  Vec3D v2{4, 5, 6};
  Vec3D v3{7, 8, 9};
  Triangle tr{v1, v2, v3};

  // Act & Assert
  EXPECT_EQ(tr[0], v1);
  EXPECT_EQ(tr[1], v2);
  EXPECT_EQ(tr[2], v3);
}

TEST(Triangle, output)
{
  // Arrange
  Vec3D v1{1, 2, 3};
  Vec3D v2{4, 5, 6};
  Vec3D v3{7, 8, 9};
  Triangle trian{v1, v2, v3};

  // Act
  std::stringstream sst{};
  sst << trian;

  // Assert
  EXPECT_STREQ(sst.str().c_str(), "Triangle: {(1, 2, 3), (4, 5, 6), (7, 8, 9)}");
}

TEST(Triangle, isValid)
{
  // Arrange
  Triangle<double> tr1{{1, 0, 0}, {0, 1, 0}, {0, 0, 0}};
  Triangle<double> tr2{{1, 0, 0}, {0, 1, 0}, {0, 1, 0}};
  Triangle<double> tr3{{1, 0, 0}, {1, 0, 0}, {1, 0, 0}};

  // Act & Assert
  EXPECT_TRUE(tr1.isValid());
  EXPECT_FALSE(tr2.isValid());
  EXPECT_FALSE(tr3.isValid());
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
