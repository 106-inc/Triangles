#include <gtest/gtest.h>

#include "geom/line.hh"

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

// TEST(Line, getBy2P)
// {
//   // Arrange
//   geom::Vector<double> p1{1, 0, 0};
//   geom::Vector<double> p2{0, 1, 0};

//   // Act
//   auto l = geom::Line<double>::getBy2P(p1, p2);

//   // Assert

// }

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
