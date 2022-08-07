#include "utils/utils.hh"

#include "test_header.hh"
#include <algorithm>
#include <numeric>
#include <vector>

using namespace utils;

TEST(EnumerTest, ordVecSimpleBind)
{
  // Arrange
  std::vector<int> vec;
  vec.resize(10);
  std::iota(vec.begin(), vec.end(), 0);

  // Act && Asserts
  for (auto val : makeEnumerate(vec))
    ASSERT_EQ(val.first, val.second);
}

TEST(EnumerTest, ordVecStructBind)
{
  // Arrange
  std::vector<int> vec;
  vec.resize(10);
  std::iota(vec.begin(), vec.end(), 0);

  // Act && Asserts
  for (auto [i, val] : makeEnumerate(vec))
    ASSERT_EQ(i, val);
}

TEST(EnumerTest, ordVecChangeValue)
{
  // Arrange
  std::vector<int> vec, expect;
  vec.resize(10);
  expect.resize(vec.size());
  std::iota(vec.begin(), vec.end(), 0);
  std::iota(expect.rbegin(), expect.rend(), -expect.size() + 1);

  // Act
  for (auto [i, val] : makeEnumerate(vec))
    val = -static_cast<int>(i);

  // Assert
  EXPECT_EQ(vec, expect);
}

TEST(EnumerTest, twiceDeref)
{
  // Arrange
  std::vector<int> vec = {1, 2, 3};

  // Act
  auto enumerate = makeEnumerate(vec);
  auto it1 = enumerate.begin();
  auto v1 = *it1;
  ++it1;
  auto v2 = *it1;

  // Assert
  EXPECT_EQ(v1.first, 0);
  EXPECT_EQ(v2.first, 1);
}

TEST(EnumerTest, ordVecTemp)
{
  // Arrange
  std::vector<int> res, expect(10, 10);
  res.resize(10);

  // Act
  for (auto [i, val] : makeEnumerate(std::vector<int>(expect.size(), 10)))
    res[i] = val;

  // Assert
  EXPECT_EQ(res, expect);
}

TEST(EnumerTest, arrowProxy)
{
  // Arrange
  struct Compl
  {
    int a;

    Compl &operator=(int v)
    {
      a = v;
      return *this;
    }

    bool operator==(const Compl &) const = default;

    void inc()
    {
      a++;
    }
  };

  std::vector<Compl> vec, expect;
  vec.resize(10);
  expect.resize(vec.size());
  std::iota(vec.begin(), vec.end(), 0);
  std::iota(expect.begin(), expect.end(), 1);

  // Act
  auto enumerate = makeEnumerate(vec);
  for (auto it = enumerate.begin(); it != enumerate.end(); it++)
    it->second.inc();

  // Assert
  EXPECT_EQ(vec, expect);
}

TEST(EnumerTest, MoveSemantics)
{
  // Arrangse
  struct WasMovedType
  {};
  struct WasCopiedType
  {};
  struct ToMove
  {
  private:
    std::vector<int> dummy{};

  public:
    ToMove() = default;

    ToMove(const ToMove &that) : dummy(that.dummy)
    {
      throw WasCopiedType{};
    }
    ToMove(ToMove &&that) : dummy(std::move(that.dummy))
    {
      throw WasMovedType{};
    }

    auto size() const
    {
      return dummy.size();
    }

    auto begin()
    {
      return dummy.begin();
    }

    auto end()
    {
      return dummy.end();
    }
  };

  ToMove local;
  // Act && Assert
  EXPECT_THROW([[maybe_unused]] auto moved = makeEnumerate(ToMove{});, WasMovedType);
  EXPECT_NO_THROW([[maybe_unused]] auto copied = makeEnumerate(local););
}

#include "test_footer.hh"
