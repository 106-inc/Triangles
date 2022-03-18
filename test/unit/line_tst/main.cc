#include <gtest/gtest.h>

#include "primitives/primitives.hh"

TEST(Line, copyCtor)
{
  // Arrange
  geom::Line<double> l1{{1, 1, 1}, {2, 2, 2}};
  auto l2{l1};
  auto l3 = l1;

  // Act & Assert
  ASSERT_EQ(l1, l2);
  ASSERT_EQ(l1, l3);
}

TEST(Line, getset)
{
  // Arrange
  geom::Vec3<double> v1{1, 0, 0};
  geom::Vec3<double> v2{0, 1, 0};
  geom::Line<double> l{v1, v2};

  // Act
  auto org = l.org();
  auto dir = l.dir();

  // Assert
  ASSERT_EQ(org, v1);
  ASSERT_EQ(dir, v2);
}

TEST(Line, getPoint)
{
  // Arrange
  geom::Vec3<double> v1{1, 0, 0};
  geom::Vec3<double> v2{0, 1, 0};
  geom::Line<double> l1{v1, v2};
  geom::Line<double> l2{{456, 30, -68}, {-456, -30, 68}};

  // Act
  auto point = l1.getPoint(20);
  auto point2 = l1.getPoint(0);
  auto point3 = l2.getPoint(1);

  // Assert
  ASSERT_EQ(point, geom::Vec3<double>(1, 20, 0));
  ASSERT_EQ(point2, l1.org());

  ASSERT_EQ(point3, geom::Vec3<double>());
}

TEST(Line, belongs)
{
  // Arrange
  geom::Vec3<double> v1{1, 1, 2};
  geom::Vec3<double> v2{1, 1, 3};
  geom::Line<double> l{{0, 0, 1}, {1, 1, 1}};

  // Act
  bool res1 = l.belongs(v1);
  bool res2 = l.belongs(v2);

  // Assert
  ASSERT_TRUE(res1);
  ASSERT_FALSE(res2);
}

TEST(Line, isEqual)
{
  // Arrange
  geom::Line<double> l1{{0, 0, 1}, {1, 1, 1}};
  geom::Line<double> l2{{1, 1, 2}, {6, 6, 6}};
  geom::Line<double> l3{{-1, 1, 2}, {6, 6, 6}};

  // Act

  /* nothing */

  // Assert
  ASSERT_TRUE(l1.isEqual(l2));
  ASSERT_TRUE(l2.isEqual(l1));

  ASSERT_EQ(l1, l2);
  ASSERT_EQ(l2, l1);

  ASSERT_FALSE(l1.isEqual(l3));
  ASSERT_FALSE(l2.isEqual(l3));

  ASSERT_FALSE(l3.isEqual(l1));
  ASSERT_FALSE(l3.isEqual(l2));

  ASSERT_NE(l1, l3);
  ASSERT_NE(l2, l3);

  ASSERT_NE(l3, l1);
  ASSERT_NE(l3, l2);

  ASSERT_EQ(l1, l1);
  ASSERT_EQ(l2, l2);
  ASSERT_EQ(l3, l3);
}

TEST(Line, isPar)
{
  // Arrange
  geom::Line<double> l1{{0, 0, 1}, {1, 1, 1}};
  geom::Line<double> l2{{5, 3, 2}, {-1, -1, -1}};
  geom::Line<double> l3{{0, 0, 0}, {1, 0, 0}};

  // Act

  /* nothing */

  // Assert
  ASSERT_TRUE(l1.isPar(l2));
  ASSERT_TRUE(l2.isPar(l1));

  ASSERT_FALSE(l2.isPar(l3));
  ASSERT_FALSE(l1.isPar(l3));
}

TEST(Line, isSkew)
{
  // Arrange
  geom::Line<double> l1{{0, 0, 1}, {1, 1, 1}};
  geom::Line<double> l2{{253, 253, 254}, {-6, -6, -6}};
  geom::Line<double> l3{{5, 3, 2}, {-3, -1, 1}};
  geom::Line<double> l4{{0, 0, 0}, {1, 0, 0}};

  // Act

  /* nothing */

  // Assert
  ASSERT_FALSE(l1.isSkew(l2));
  ASSERT_FALSE(l2.isSkew(l1));

  ASSERT_FALSE(l1.isSkew(l3));
  ASSERT_FALSE(l3.isSkew(l1));

  ASSERT_TRUE(l1.isSkew(l4));
  ASSERT_TRUE(l4.isSkew(l1));

  ASSERT_TRUE(l2.isSkew(l4));
  ASSERT_TRUE(l4.isSkew(l2));
}

TEST(Line, getBy2Points)
{
  // Arrange
  geom::Vec3<double> p1{1, 0, 0};
  geom::Vec3<double> p2{0, 1, 0};

  // Act
  auto l = geom::Line<double>::getBy2Points(p1, p2);

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
