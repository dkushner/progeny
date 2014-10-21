#include <gtest/gtest.h>
#include <iostream>
#include <vector>

#include "../src/core/mutator.h"
#include "../src/mutators/crossover.h"

template <typename T>
class CrossoverTest: public testing::Test {
  public:
    using Candidate = pr::Candidate<T, double>;
    using Population = pr::Population<Candidate>;

  protected:
    CrossoverTest() : _crossover(new pr::Crossover<Candidate>(2)) {}
    virtual ~CrossoverTest() { delete _crossover; }

  protected:
    static Population _population;
    pr::Crossover<Candidate>* _crossover;
};

template <>
CrossoverTest<std::string>::Population 
CrossoverTest<std::string>::_population{ "wwwwww", "xxxxxx", "yyyyyy", "zzzzzz" };

template <>
CrossoverTest<std::tuple<int, double, char>>::Population 
CrossoverTest<std::tuple<int, double, char>>::_population{
  std::make_tuple(1, 1.0, 'a'),
  std::make_tuple(2, 2.0, 'b'),
  std::make_tuple(3, 3.0, 'c'),
  std::make_tuple(4, 4.0, 'd')
};

template <>
CrossoverTest<std::array<char, 5>>::Population
CrossoverTest<std::array<char, 5>>::_population{
  { 'a', 'a', 'a', 'a', 'a' },
  { 'b', 'b', 'b', 'b', 'b' },
  { 'c', 'c', 'c', 'c', 'c' },
  { 'd', 'd', 'd', 'd', 'd' }
};

using testing::Types;
typedef Types<
  std::string,
  std::array<char, 5>,
  std::tuple<int, double, char>
> Params;
TYPED_TEST_CASE(CrossoverTest, Params);

TYPED_TEST(CrossoverTest, Mutation) {
  using Population = typename CrossoverTest<TypeParam>::Population;

  Population ptest = CrossoverTest<TypeParam>::_population;
  this->_crossover->mutate(ptest);

  EXPECT_EQ(ptest.size(), this->_population.size());

  auto test_itr = ptest.begin();
  auto itr = this->_population.begin();
  for (; itr != this->_population.end(); itr++, test_itr++) {
    EXPECT_NE(pr::progeny(*test_itr), pr::progeny(*itr));
  }
 }


