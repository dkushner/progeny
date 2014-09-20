#include <gtest/gtest.h>
#include <iostream>

#include "../src/core/progeny.h"
#include "../src/generators/string_generator.h"

TEST(Generators_StringGenerator, DefaultPopulations) {
  PROGENY_TYPE(std::string);

  std::random_device rd;
  std::mt19937 rnd(rd());

  StringGenerator g(5, rnd);
  Population p = g.generate(5);

  /* Check that our population size is correct. */
  EXPECT_EQ(p.size(), 5);

  /*
  * For each member of the population, verify that it adheres
  * to our specifications.
  */
  for (auto s : p) {
    EXPECT_EQ(s.size(), 5);
  }
}

TEST(Generators_StringGenerator, ConstrainedPopulations) {
  PROGENY_TYPE(std::string);

  std::random_device rd;
  std::mt19937 rnd(rd());

  /* Valid character pool contains only 'x'. */
  StringGenerator g(2, "x", rnd);
  Population p = g.generate(5);

  EXPECT_EQ(p.size(), 5);
  for (auto s : p) {
    EXPECT_EQ(s.size(), 2);
    EXPECT_STREQ(s.c_str(), "xx");
  }
}

TEST(Generators_StringGenerator, CustomRandomGenerator) {
  PROGENY_TYPE(std::string);

  /*
   * If we construct our custom generator to always
   * return 2 we should get a population identical to
   * the previous test.
   */
  StringGenerator g(2, "abxcdefgh", []{ return 2; });
  Population p = g.generate(5);

  EXPECT_EQ(p.size(), 5);
  for (auto s : p) {
    EXPECT_EQ(s.size(), 2);
    EXPECT_STREQ(s.c_str(), "xx");
  }
}

TEST(Generators_StringGenerator, SeededPopulation) {
  PROGENY_TYPE(std::string);

  std::random_device rd;
  std::mt19937 rnd(rd());

  /* Seed tuples. */
  Progeny p1("aa");
  Progeny p2("bb");

  StringGenerator g(2, "cde", rnd);
  Population seed { p1, p2 };
  Population p = g.generate(seed, 5);

  auto found1 = std::find(std::begin(p), std::end(p), p1);
  auto found2 = std::find(std::begin(p), std::end(p), p2);

  /*
   * Validate that our two seed members appear in the
   * final population.
   */
  EXPECT_EQ(p.size(), 5);
  EXPECT_TRUE(found1 != std::end(p));
  EXPECT_TRUE(found2 != std::end(p));
}
