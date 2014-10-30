#include <gtest/gtest.h>
#include <iostream>

#include "../src/generators/fill_generator.h"

TEST(FillGenerator, NoSeed) {

  using Candidate = pr::Candidate<int, double>;
  using Population = pr::Population<Candidate>;

  pr::FillGenerator<Candidate> gen([&]{
    return 5;
  });

  Population pop;
  gen.generate(pop, 10);

  EXPECT_EQ(pop.size(),10);
  for (auto& m : pop) {
    EXPECT_EQ(pr::progeny(m), 5);
  }
}

TEST(FillGenerator, Fill) {
  using Candidate = pr::Candidate<int, double>;
  using Population = pr::Population<Candidate>;

  pr::FillGenerator<Candidate> gen([&]{
    return 5;
  });

  Population pop{1, 1, 1, 1};
  gen.generate(pop, 10);

  EXPECT_EQ(pop.size(), 10);

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

  EXPECT_EQ(ones, 4);
  EXPECT_EQ(fives, 6);

}
