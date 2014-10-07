#include <gtest/gtest.h>
#include <tuple>
#include <map>
#include <iostream>
#include <string>

#include "../src/core/population.h"
#include "../src/evaluators/string_evaluator.h"

TEST(Evaluators, StringEvaluator) {

  using Candidate = pr::Candidate<std::string, double>;
  using Population = pr::Population<Candidate>;

  Population pop { "duck", "duck", "duck", "duck", "goose" };
  StringEvaluator<double> sev("goose");

  EXPECT_EQ(rp.size(), p.size());
  for (auto pair : rp) {
    EXPECT_EQ(pair.second, expected[pair.first]);
  }
}

/*
TEST(Evaluators, NullEvaluator) {
  PROGENY_TYPE(std::string);

  NullEvaluator<std::string> nev;

  Progeny p1("red");
  Progeny p2("green");
  Progeny p3("blue");

  Population p { p1, p2, p3 };
  RankedPopulation rp = nev.evaluate(p);

  EXPECT_EQ(rp.size(), p.size());
  for (auto pair : rp) {
    EXPECT_EQ(pair.second, 0.0);
  }
}
*/
