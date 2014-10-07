#include <gtest/gtest.h>
#include <iostream>


/*
TEST(StringGenerator, DefaultPopulations) {
  PROGENY_TYPE(std::string);

  StringGenerator g(5);
  Population p = g.generate(5);

  EXPECT_EQ(p.size(), 5);

  for (auto s : p) {
    EXPECT_EQ(s.size(), 5);
  }
}

TEST(StringGenerator, ConstrainedPopulations) {
  PROGENY_TYPE(std::string);

  StringGenerator g(2, "x");
  Population p = g.generate(5);

  EXPECT_EQ(p.size(), 5);
  for (auto s : p) {
    EXPECT_EQ(s.size(), 2);
    EXPECT_STREQ(s.c_str(), "xx");
  }
}

TEST(StringGenerator, SeededPopulation) {
  PROGENY_TYPE(std::string);

  Progeny p1("aa");
  Progeny p2("bb");

  StringGenerator g(2, "cde");
  Population seed { p1, p2 };
  Population p = g.generate(seed, 5);

  auto found1 = std::find(std::begin(p), std::end(p), p1);
  auto found2 = std::find(std::begin(p), std::end(p), p2);

  EXPECT_EQ(p.size(), 5);
  EXPECT_TRUE(found1 != std::end(p));
  EXPECT_TRUE(found2 != std::end(p));
}
*/
