#include <gtest/gtest.h>
#include <tuple>
#include <map>
#include <iostream>
#include <string>

#include "../src/core/progeny.h"
#include "../src/evaluators/null_evaluator.h"
#include "../src/evaluators/string_evaluator.h"

TEST(Evaluators, StringEvaluator) {
  PROGENY_TYPE(std::string);

  StringEvaluator sev("target");

  Progeny p1("tarfir");
  Progeny p2("dfcaet");
  Progeny p3("target");

  Population p { p1, p2, p3 };
  RankedPopulation rp = sev.evaluate(p);
  std::map<Progeny, double> expected {
    std::make_pair(p1, 3.0 / 6.0),
    std::make_pair(p2, 2.0 / 6.0),
    std::make_pair(p3, 1.0)
  };

  EXPECT_EQ(rp.size(), p.size());
  for (auto pair : rp) {
    EXPECT_EQ(pair.second, expected[pair.first]);
  }
}

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
    EXPECT_EQ(pair.second, 0);
  }
}
