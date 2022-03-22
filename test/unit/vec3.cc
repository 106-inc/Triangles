#include "primitives/primitives.hh"
#include "test_common.hh"

using namespace geom;

template <typename T>
class Vec3Test : public testing::Test
{};

TYPED_TEST_SUITE(Vec3Test, FPTypes);

TYPED_TEST(Vec3Test, copyCtor)
{
  // Arrange
  Vec3<TypeParam> v1{1, 2, 3};
  Vec3<TypeParam> v2 = v1;
  Vec3<TypeParam> v3{v1};

  // Act & Arrange
  EXPECT_EQ(v1, v2);
  EXPECT_EQ(v1, v3);
}

TYPED_TEST(Vec3Test, dot)
{
  // Arrange
  Vec3<TypeParam> v1{1, 2, 3};
  Vec3<TypeParam> v2{4, 5, 6};

  // Act
  auto res = v1.dot(v2);

  // Assert
  EXPECT_NEAR(res, 32, Vec3<TypeParam>::getThreshold());
}

TYPED_TEST(Vec3Test, isEq)
{
  // Arrange
  Vec3<TypeParam> v1{1.0, 2.0, 3.0};
  Vec3<TypeParam> v2{1.0, 2.0, 3.0};
  Vec3<TypeParam> v3{4.0, 5.0, 5.0};

  // Act & Assert
  EXPECT_TRUE(v1.isEqual(v2));
  EXPECT_TRUE(v2.isEqual(v1));

  EXPECT_FALSE(v1.isEqual(v3));
  EXPECT_FALSE(v2.isEqual(v3));

  EXPECT_FALSE(v3.isEqual(v1));
  EXPECT_FALSE(v3.isEqual(v2));
}

TYPED_TEST(Vec3Test, cross)
{
  // Arrange
  Vec3<TypeParam> v1{1, 2, 3};
  Vec3<TypeParam> v2{4, 5, 6};

  // Act
  auto res = v1.cross(v2);

  // Assert
  EXPECT_TRUE(res.isEqual(Vec3<TypeParam>{-3, 6, -3}));
}

TYPED_TEST(Vec3Test, triple)
{
  // Arrange
  Vec3<TypeParam> v1{1, 2, 3};
  Vec3<TypeParam> v2{4, 5, 6};
  Vec3<TypeParam> v3{7, 8, 9};

  Vec3<TypeParam> v4{1, 0, 0};
  Vec3<TypeParam> v5{0, 1, 0};
  Vec3<TypeParam> v6{0, 0, 1};

  // Act
  auto res = triple(v1, v2, v3);
  auto nres = triple(v2, v1, v3);

  auto res2 = triple(v4, v5, v6);
  auto nres2 = triple(v6, v5, v4);

  // Assert
  EXPECT_TRUE(Vec3<TypeParam>::isNumEq(res, 0));
  EXPECT_TRUE(Vec3<TypeParam>::isNumEq(nres, 0));

  EXPECT_TRUE(Vec3<TypeParam>::isNumEq(res2, 1));
  EXPECT_TRUE(Vec3<TypeParam>::isNumEq(nres2, -1));
}

TYPED_TEST(Vec3Test, normalize)
{
  // Arrange
  Vec3<TypeParam> v1{2, 6, 9};
  Vec3<TypeParam> v2{0, 0, 0};
  Vec3<TypeParam> v3{0, 0, 1};

  // Act
  auto res1 = v1.normalized();
  auto res2 = v2.normalized();
  auto res3 = v3.normalized();

  // Assert
  EXPECT_TRUE(res1.isEqual(v1 / 11));
  EXPECT_TRUE(res2.isEqual(v2));
  EXPECT_TRUE(res3.isEqual(v3));
}

TYPED_TEST(Vec3Test, isPar)
{
  // Arrange
  Vec3<TypeParam> v1{2, -6, 9};
  Vec3<TypeParam> v2{8, -24, 36};
  Vec3<TypeParam> v3{6, 4, 1};

  // Act
  auto res12 = v1.isPar(v2);
  auto res13 = v1.isPar(v3);
  auto res23 = v3.isPar(v2);

  // Assert
  EXPECT_TRUE(res12);
  EXPECT_FALSE(res13);
  EXPECT_FALSE(res23);
}

TYPED_TEST(Vec3Test, isPerp)
{
  // Arrange
  Vec3<TypeParam> v1{1, 1, 0};
  Vec3<TypeParam> v2{-1, 1, 0};
  Vec3<TypeParam> v3{0, 0, 1};
  Vec3<TypeParam> v4{1, 1, 1};
  Vec3<TypeParam> vz{0};

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
