#include <gtest/gtest.h>

#include "primitives/line.hh"

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
  geom::Vector<double> v1{1, 0, 0};
  geom::Vector<double> v2{0, 1, 0};
  geom::Line<double> l{v1, v2};

  // Act
  auto org = l.org();
  auto dir = l.dir();

  // Assert
  ASSERT_EQ(org, v1);
  ASSERT_EQ(dir, v2);
}

TEST(Line, belongs)
{
  // Arrange
  geom::Vector<double> v1{1, 1, 2};
  geom::Vector<double> v2{1, 1, 3};
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

TEST(Line, getBy2Points)
{
  // Arrange
  geom::Vector<double> p1{1, 0, 0};
  geom::Vector<double> p2{0, 1, 0};

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
