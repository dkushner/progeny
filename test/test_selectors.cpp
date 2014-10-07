#include <gtest/gtest.h>
#include <iostream>

/*
TEST(Selectors, RouletteSelector) {

  PROGENY_TYPE(std::string);

  StringEvaluator sev("perfect");
  RouletteSelector<std::string> rs;

  Population p(5);
  std::fill_n(p.begin(), 4, "thiswordiswaytoolongbro");
  p[4] = "perfect";

  Population sel = rs.select(sev.evaluate(p), 3, false);

  EXPECT_EQ(3, sel.size());

  int count = 0;
  for (auto pr : sel) {
    if (pr == "perfect") {
      count++;
    }
  }
  EXPECT_GE(count, 2);
}
*/
