#include "utils/utils.hh"

#include "test_header.hh"
#include <algorithm>
#include <numeric>
#include <vector>

using namespace utils;

TEST(EnumerateTest, ordVecSimpleBind)
{
  // Arrange
  std::vector<int> vec;
  vec.resize(10);
  std::iota(vec.begin(), vec.end(), 0);

  // Act && Asserts
  for (auto val : Enumerate(vec))
    ASSERT_EQ(val.first, val.second);
}

TEST(EnumerateTest, ordVecStructBind)
{
  // Arrange
  std::vector<int> vec;
  vec.resize(10);
  std::iota(vec.begin(), vec.end(), 0);

  // Act && Asserts
  for (auto [i, val] : Enumerate(vec))
    ASSERT_EQ(i, val);
}

TEST(EnumerateTest, ordVecChangeValue)
{
  // Arrange
  std::vector<int> vec, expect;
  vec.resize(10);
  expect.resize(vec.size());
  std::iota(vec.begin(), vec.end(), 0);
  std::iota(expect.rbegin(), expect.rend(), -expect.size() + 1);

  // Act
  for (auto [i, val] : Enumerate(vec))
    val = -static_cast<int>(i);

  // Assert
  EXPECT_EQ(vec, expect);
}

TEST(EnumerateTest, twiceDeref)
{
  // Arrange
  std::vector<int> vec = {1, 2, 3};

  // Act
  auto enumerate = Enumerate(vec);
  auto it1 = enumerate.begin();
  auto it2 = enumerate.begin();

  using value_type =
    std::iterator_traits<detail::EnumerateIt<std::vector<int>::iterator>>::value_type;

  value_type v1 = *it1++;
  value_type v2 = *it1;

  auto r1 = *it2;
  ++it2;
  auto r2 = *it2;

  // Assert
  EXPECT_EQ(v1.first, 0);
  EXPECT_EQ(v2.first, 1);

  EXPECT_NE(r1, r2);
}

TEST(EnumerateTest, ordVecTemp)
{
  // Arrange
  std::vector<int> res, expect(10, 10);
  res.resize(10);

  // Act
  for (auto [i, val] : Enumerate(std::vector<int>(expect.size(), 10)))
    res[i] = val;

  // Assert
  EXPECT_EQ(res, expect);
}

TEST(EnumerateTest, arrowProxy)
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
  auto enumerate = Enumerate(vec);
  for (auto it = enumerate.begin(); it != enumerate.end(); it++)
    it->second.inc();

  // Assert
  EXPECT_EQ(vec, expect);
}

TEST(EnumerateTest, constContainer)
{
  // Arrange
  const std::vector vec = {1, 2, 3, 4, 5};
  std::vector<int> vec2(vec.size());

  // Act
  for (auto [idx, val] : Enumerate(vec))
    vec2[idx] = val;

  // Assert
  EXPECT_EQ(vec, vec2);
}

TEST(EnumerateTest, MoveSemantics)
{
  // Arrange
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
  EXPECT_THROW([[maybe_unused]] auto moved = Enumerate(ToMove{});, WasMovedType);
  EXPECT_NO_THROW([[maybe_unused]] auto copied = Enumerate(local););
}

#include "test_footer.hh"
