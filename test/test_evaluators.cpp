#include <gtest/gtest.h>
#include <tuple>
#include <map>
#include <iostream>
#include <string>

#include "../src/core/population.h"
#include "../src/evaluators/null_evaluator.h"
#include "../src/evaluators/mismatch_evaluator.h"
#include "../src/evaluators/competitive_evaluator.h"

template <typename T>
class MismatchTest : public testing::Test {

  public:
    using Candidate = pr::Candidate<T, double>;
    using Population = pr::Population<Candidate>;

  protected:
    MismatchTest() : 
      _mismatch(new pr::MismatchEvaluator<Candidate>(_proto)) {}
    virtual ~MismatchTest() { delete _mismatch; }

  protected:
    static const T _proto;
    static Population _population;
    pr::MismatchEvaluator<Candidate>* _mismatch;
};

// Const value initialization for the test fixture.
// There is _definitely_ a better way to do this.
template <> 
const std::string 
MismatchTest<std::string>::_proto = "goose";

template <> 
MismatchTest<std::string>::Population MismatchTest<std::string>::_population{ 
  "duck", "duck", "duck", "duck", "goose"
};

template <> 
const std::tuple<int, double, char>
MismatchTest<std::tuple<int, double, char>>::_proto{99, 3.14, 'c'}; 

template <> 
MismatchTest<std::tuple<int, double, char>>::Population
MismatchTest<std::tuple<int, double, char>>::_population{
  std::make_tuple(98, 3.0, 'a'),
  std::make_tuple(98, 3.0, 'a'),
  std::make_tuple(98, 3.0, 'a'),
  std::make_tuple(98, 3.0, 'a'),
  std::make_tuple(99, 3.14, 'c')
};

template <> 
const std::array<char, 5>
MismatchTest<std::array<char, 5>>::_proto({ 'g', 'o', 'o', 's', 'e' });

template <> 
MismatchTest<std::array<char, 5>>::Population 
MismatchTest<std::array<char, 5>>::_population{ 
  { 'd', 'u', 'c', 'k', 'k' },
  { 'd', 'u', 'c', 'k', 'k' },
  { 'd', 'u', 'c', 'k', 'k' },
  { 'd', 'u', 'c', 'k', 'k' },
  { 'g', 'o', 'o', 's', 'e' }
};

using testing::Types;
typedef Types<
  std::string,
  std::array<char, 5>,
  std::tuple<int, double, char>
> Params;
TYPED_TEST_CASE(MismatchTest, Params);

TYPED_TEST(MismatchTest, Evaluation) {
  this->_mismatch->evaluate(this->_population);

  auto it = this->_population.begin();
  for(; it != this->_population.end() - 1; it++) {
    // Non-identical candidates should accrue some error points.
    EXPECT_NE(pr::fitness(*it), 0.0);
  }
  EXPECT_EQ(pr::fitness(*it), 0.0);
}

TEST(Evaluators, NullEvaluator) {
  using Candidate = pr::Candidate<int, double>;
  using Population = pr::Population<Candidate>;

  Population pop{ 1, 2, 3, 4, 5 };
  pr::NullEvaluator<Candidate> nev;
  nev.evaluate(pop);

  EXPECT_EQ(pop.size(), 5); 
  for (auto it = pop.begin(); it != pop.end(); it++) {
    EXPECT_EQ(pr::fitness(*it), 0.0);
  }
}

TEST(Evaluators, CompetitiveEvaluator) {
  using Candidate = pr::Candidate<int, double>;
  using Population = pr::Population<Candidate>;
  using PopItr = Population::iterator;

  Population pop{ 1, 2, 3, 4, 5, 6 };
  pr::CompetitiveEvaluator<Candidate, 3> cev([](PopItr first, PopItr last){
    EXPECT_EQ(std::distance(first, last), 3);

    for (; first != last; first++) {
      pr::fitness(*first) = 5.0;
    }
  });

  cev.evaluate(pop);
  for (auto& m : pop) {
    EXPECT_EQ(pr::fitness(m), 5.0);
  }

}
