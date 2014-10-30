#include <gtest/gtest.h>
#include <iostream>
#include <vector>

#include "../src/core/mutator.h"
#include "../src/mutators/crossover.h"
#include "../src/mutators/pass_through.h"

template <typename T>
class CrossoverTest: public testing::Test {
  public:
    using Candidate = pr::Candidate<T, double>;
    using Population = pr::Population<Candidate>;

  protected:
    CrossoverTest() : _crossover(new pr::Crossover<Candidate>(2)) {}
    virtual ~CrossoverTest() { delete _crossover; }

    static int countCrossovers(T& t);

  protected:
    static Population _population;
    pr::Crossover<Candidate>* _crossover;
};

template <>
CrossoverTest<std::string>::Population 
CrossoverTest<std::string>::_population{ "wwwwww", "xxxxxx", "yyyyyy", "zzzzzz" };

template <>
CrossoverTest<std::tuple<int, double, float>>::Population 
CrossoverTest<std::tuple<int, double, float>>::_population{
  std::make_tuple(1, 1.0, 1.0),
  std::make_tuple(2, 2.0, 1.0),
  std::make_tuple(3, 3.0, 1.0),
  std::make_tuple(4, 4.0, 1.0)
};

template <>
CrossoverTest<std::array<char, 5>>::Population
CrossoverTest<std::array<char, 5>>::_population{
  { 'a', 'a', 'a', 'a', 'a' },
  { 'b', 'b', 'b', 'b', 'b' },
  { 'c', 'c', 'c', 'c', 'c' },
  { 'd', 'd', 'd', 'd', 'd' }
};

template <>
int CrossoverTest<std::string>::countCrossovers(std::string& t) {
  auto tail = t.begin();
  auto head = tail + 1;

  int switches = 0;
  for (; head != t.end(); head++, tail++) {
    if (*head != *tail) {
      switches++;
    }
  }
  return switches;
}

template <>
int CrossoverTest<std::tuple<int, double, float>>::countCrossovers(std::tuple<int, double, float>& t) {
  int switches = 0;
  if (std::get<0>(t) != std::get<1>(t)) {
    switches++;
  }

  if (std::get<1>(t) != std::get<2>(t)) {
    switches++;
  }
  return switches;
}

template <>
int CrossoverTest<std::array<char, 5>>::countCrossovers(std::array<char, 5>& t) {
  auto tail = std::begin(t);
  auto head = tail + 1;

  int switches = 0;
  for (; head != t.end(); head++, tail++) {
    if (*head != *tail) {
      switches++;
    }
  }

  return switches;
}

using testing::Types;
typedef Types<
  std::string,
  std::array<char, 5>,
  std::tuple<int, double, float>
> Params;
TYPED_TEST_CASE(CrossoverTest, Params);

      
TYPED_TEST(CrossoverTest, Mutation) {
  using Population = typename CrossoverTest<TypeParam>::Population;

  Population ptest = CrossoverTest<TypeParam>::_population;
  this->_crossover->mutate(ptest);

  EXPECT_EQ(ptest.size(), this->_population.size());

  auto itr = this->_population.begin();
  for (; itr != this->_population.end(); itr++) {
    int switches = this->countCrossovers(pr::progeny(*itr));
    EXPECT_LE(switches, 2);
  }
}

TEST(PassThrough, Mutation) {
  using Candidate = pr::Candidate<int, double>;
  using Population = pr::Population<Candidate>;

  Population pop{1, 2, 3, 4, 5};
  Population tpop(pop);
  pr::PassThrough<Candidate> pt;

  pt.mutate(pop);

  EXPECT_EQ(pop.size(), 5);
  auto itr_a = pop.begin();
  auto itr_b = tpop.begin();
  for (; itr_a != pop.end(); itr_a++, itr_b++) {
    EXPECT_EQ(pr::progeny(*itr_a), pr::progeny(*itr_b));
  }
}

TEST(Pipeline, Mutation) {
  using Candidate = pr::Candidate<std::string, double>;
  using Population = pr::Population<Candidate>;

  Population pop{ "cool", "stuff", "happens", "october" };

  auto mut = pr::Crossover<Candidate>(2) >> pr::PassThrough<Candidate>();
  mut.mutate(pop);

  EXPECT_EQ(pop.size(), 4);
}


