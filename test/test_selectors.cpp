#include <gtest/gtest.h>
#include <iostream>

#include "../src/core/progeny.h"
#include "../src/core/selector.h"
#include "../src/selectors/roulette_selector.h"
#include "../src/evaluators/string_evaluator.h"

TEST(Selectors, RouletteSelector) {

  PROGENY_TYPE(std::string);

  StringEvaluator sev("target");
  RouletteSelector<std::string> rs;

  Progeny p1("short");
  Progeny p2("short");
  Progeny p3("target");

  Population p { p1, p2, p3 };
  Population sel = rs.select(std::move(sev.evaluate(p)), 3);

  EXPECT_EQ(p.size(), sel.size());
  for (auto pr : sel) {
    EXPECT_EQ("target", pr);
  }
}
