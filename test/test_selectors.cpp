#include <gtest/gtest.h>
#include <iostream>

#include "../src/selectors/roulette_selector.h"

TEST(Selectors, RouletteSelector) {
  using Candidate = pr::Candidate<int, double>; 
  using Population = pr::Population<Candidate>;

  Population pop{
    {1, 0.0},
    {2, 0.0},
    {3, 0.0},
    {4, 1.0},
    {5, 1.0},
    {6, 1.0},
  };

  pr::RouletteSelector<Candidate> rs;
  rs.select(pop, 2);

  EXPECT_EQ(pop.size(), 6);

  int alive_count = 0;
  for (auto& m : pop) {
    if (m.alive) {
      EXPECT_GE(pr::progeny(m), 4);
      alive_count++;
    }
  }

  EXPECT_EQ(alive_count, 2);
}
