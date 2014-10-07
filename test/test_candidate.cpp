#include <gtest/gtest.h>
#include <string>
#include <iostream>

#include "../src/core/candidate.h"

template <typename T>
class CandidateTest : public testing::Test {

  protected:
    CandidateTest() : 
      _candidate(new Candidate<T, double>(_value, 0.0)) {}
    virtual ~CandidateTest() { delete _candidate; }

  protected:
    static const T _value;
    Candidate<T, double>* _candidate;
};

template <>
const std::string CandidateTest<std::string>::_value = "test";

template <>
const std::tuple<int, double, char> CandidateTest<
  std::tuple<int, double, char>
>::_value { 99, 3.14, 'c' };

template <>
const std::array<char, 5> CandidateTest<std::array<char, 5>>::_value = {
  'a', 'b', 'c', 'd', 'e'
};


using testing::Types;
typedef Types<
  std::string, 
  std::array<char, 5>, 
  std::tuple<int, double, char>
> Params;
TYPED_TEST_CASE(CandidateTest, Params);

TYPED_TEST(CandidateTest, Constructor) {
  EXPECT_EQ(this->_candidate->member(), CandidateTest<TypeParam>::_value);
  EXPECT_EQ(this->_candidate->fitness(), 0.0);
}
