#include <gtest/gtest.h>
#include <tuple>
#include <map>
#include <iostream>
#include <string>

#include "../src/core/population.h"
#include "../src/evaluators/null_evaluator.h"
#include "../src/evaluators/mismatch_evaluator.h"

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
    static const Candidate _proto;
    static Population _population;
    pr::MismatchEvaluator<Candidate>* _mismatch;
};

// Const value initialization for the test fixture.
// There is _definitely_ a better way to do this.
template <> 
const MismatchTest<std::string>::Candidate 
MismatchTest<std::string>::_proto{ "goose" };

template <> 
MismatchTest<std::string>::Population MismatchTest<std::string>::_population{ 
  "duck", "duck", "duck", "duck", "goose"
};

template <> 
const MismatchTest<std::tuple<int, double, char>>::Candidate
MismatchTest<std::tuple<int, double, char>>::_proto { 
  std::make_tuple(99, 3.14, 'c')
};

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
const MismatchTest<std::array<char, 5>>::Candidate 
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
  auto it = this->_population.begin();
  for(; it != this->_population.end() - 1; it++) {

    this->_mismatch->evaluate(*it);
    // Non-identical candidates should accrue some error points.
    EXPECT_NE(pr::fitness(*it), 0.0);
  }
  EXPECT_EQ(pr::fitness(*it), 0.0);
}

TEST(Evaluators, NullEvaluator) {
  using Candidate = pr::Candidate<int, double>;
  using Population = pr::Population<Candidate>;

  Population pop{ 1, 2, 3, 4, 5};
  pr::NullEvaluator<Candidate> nev;

  EXPECT_EQ(pop.size(), 5); 
  for (auto it = pop.begin(); it != pop.end(); it++) {
    nev.evaluate(*it);
    EXPECT_EQ(pr::fitness(*it), 0.0);
  }
}
