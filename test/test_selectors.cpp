#include <gtest/gtest.h>
#include <iostream>

#include "../src/selectors/roulette_selector.h"

TEST(Selectors, RouletteSelector) {
  using Candidate = pr::Candidate<int, double>; 
  using Population = pr::Population<Candidate>;

  Population pop{
    {1, 0.1},
    {2, 0.1},
    {3, 0.1},
    {4, 99.0},
    {5, 99.0},
    {6, 99.0},
  };

  pr::RouletteSelector<Candidate> rs;
  rs.select(pop, 2);

  EXPECT_EQ(pop.size(), 2);

  for (auto& m : pop) {
    EXPECT_GE(pr::progeny(m), 4);
  }
}
