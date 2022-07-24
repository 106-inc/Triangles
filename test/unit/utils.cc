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

TEST(EnumerTest, ordVecChangeCounter)
{
  // Arrange
  std::vector<int> vec;
  vec.resize(10);
  std::iota(vec.begin(), vec.end(), 0);

  // Act && Asserts
  for (auto [i, val] : makeEnumerate(vec))
    if (i == 0)
      i = 22;
    else
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
  ASSERT_EQ(vec, expect);
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
  ASSERT_EQ(res, expect);
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
  ASSERT_EQ(vec, expect);
}

#include "test_footer.hh"
