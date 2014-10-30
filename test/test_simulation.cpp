#include <gtest/gtest.h>
#include <iostream>
#include <vector>

#include "../src/core/simulation.h"
#include "../src/core/candidate.h"
#include "../src/core/population.h"

#include "../src/evaluators/mismatch_evaluator.h"
#include "../src/generators/fill_generator.h"
#include "../src/selectors/roulette_selector.h"
#include "../src/mutators/pass_through.h"
#include "../src/mutators/crossover.h"

TEST(Simulation, Builder) {
  using Candidate = pr::Candidate<std::string, double>;
  using Population = pr::Population<Candidate>;
  using PopItr = Population::iterator;

  // Construct Generator
  pr::FillGenerator<Candidate> fg([]{
    std::string str(5, 0);
    std::generate(str.begin(), str.end(), []{
      const char valid[] = "abcdefghijklmnopqrstuvwxyz";
      return valid[rand() % 26];
    });
    return str;
  });

  // Construct Evaluator
  pr::MismatchEvaluator<Candidate> mev("david");

  // Construct Selector
  pr::RouletteSelector<Candidate> rs;

  // Construct Mutator
  auto mut = pr::Crossover<Candidate>(2) >> pr::PassThrough<Candidate>();

  // Finally, compose the simulator instance.
  auto sim = pr::Simulation<Candidate>::build(fg, mev, rs, mut);

}
