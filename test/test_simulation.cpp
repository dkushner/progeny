#include <gtest/gtest.h>
#include <iostream>
#include <vector>

#include "../src/core/simulation.h"
#include "../src/core/candidate.h"
#include "../src/core/population.h"

#include "../src/evaluators/competitive_evaluator.h"
#include "../src/evaluators/roulette_selector.h"

TEST(Simulation, Builder) {
  using Candidate = pr::Candidate<int, double>;
  using Population = pr::Population<Candidate>;
  using PopItr = Population::iterator;

  // Construct Generator
  pr::FillGenerator<Candidate> fg([]{
    return 5;
  });

  // Construct Evaluator
  pr::CompetitiveEvaluator<Candidate, 2> cev([](PopItr start, PopItr end) {
    for (; start != end; start++) {
      if (pr::progeny(*start) > pr::progeny(*end)) {
        pr::fitness(*start) += (pr::progeny(*end) / 2);
        pr::fitness(*end) -= (pr::progeny(*start) / 2);
      } else {
        pr::fitness(*start) -= (pr::progeny(*end) / 2);
        pr::fitness(*end) += (pr::progeny(*start) / 2);
      }
    }
  });

  // Construct Selector
  pr::RouletteSelector<Candidate> rs;

  // Construct Mutator

}
