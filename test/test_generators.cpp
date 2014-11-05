#include <gtest/gtest.h>
#include <iostream>

#include "../src/generators/fill_generator.h"

TEST(Generators, Replace) {
  using Candidate = pr::Candidate<int, double>;
  using Population = pr::Population<Candidate>;

  pr::FillGenerator<Candidate> gen([&]{
    return 5;
  });

  Population pop{1, 1, 1, 1, 1};
  for (int i = 0; i < 3; ++i) {
    pop[i].alive = false;
  }
  gen.generate(pop);

  int ones = 0;
  int fives = 0;
  for (auto& m : pop) {
    if (pr::progeny(m) == 1) {
      ones++;
    }

    if (pr::progeny(m) == 5) {
      fives++;
    }
  }

  EXPECT_EQ(ones, 2);
  EXPECT_EQ(fives, 3);

}
