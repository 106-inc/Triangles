#include <sstream>

#include "primitives/primitives.hh"
#include "test_header.hh"

using namespace geom;

template <typename T>
class TriangleTest : public testing::Test
{};

TYPED_TEST_SUITE(TriangleTest, FPTypes);

TYPED_TEST(TriangleTest, ctor)
{
  // Arrange
  Vec3<TypeParam> v1{1, 2, 3};
  Vec3<TypeParam> v2{4, 5, 6};
  Vec3<TypeParam> v3{7, 8, 9};
  Triangle tr{v1, v2, v3};

  // Act & Assert
  EXPECT_EQ(tr[0], v1);
  EXPECT_EQ(tr[1], v2);
  EXPECT_EQ(tr[2], v3);
}

TYPED_TEST(TriangleTest, output)
{
  // Arrange
  Vec3<TypeParam> v1{1, 2, 3};
  Vec3<TypeParam> v2{4, 5, 6};
  Vec3<TypeParam> v3{7, 8, 9};
  Triangle trian{v1, v2, v3};

  // Act
  std::stringstream sst{};
  sst << trian;

  // Assert
  EXPECT_STREQ(sst.str().c_str(), "Triangle: {(1, 2, 3), (4, 5, 6), (7, 8, 9)}");
}

TYPED_TEST(TriangleTest, isValid)
{
  // Arrange
  Triangle<TypeParam> tr1{{1, 0, 0}, {0, 1, 0}, {0, 0, 0}};
  Triangle<TypeParam> tr2{{1, 0, 0}, {0, 1, 0}, {0, 1, 0}};
  Triangle<TypeParam> tr3{{1, 0, 0}, {1, 0, 0}, {1, 0, 0}};

  // Act & Assert
  EXPECT_TRUE(tr1.isValid());
  EXPECT_FALSE(tr2.isValid());
  EXPECT_FALSE(tr3.isValid());
}

TYPED_TEST(TriangleTest, boundBox)
{
  // Arrange
  Triangle<TypeParam> tr{{-3, 4, 0}, {1, 1, -5}, {6, 0, 5}};
  BoundBox<TypeParam> bb{-3 - ThresComp<TypeParam>::getThreshold(), 6 + ThresComp<TypeParam>::getThreshold(),
                         0 - ThresComp<TypeParam>::getThreshold(),  4 + ThresComp<TypeParam>::getThreshold(),
                         -5 - ThresComp<TypeParam>::getThreshold(), 5 + ThresComp<TypeParam>::getThreshold()};

  // Act & Assert
  EXPECT_EQ(tr.boundBox(), bb);
}

#include "test_footer.hh"
