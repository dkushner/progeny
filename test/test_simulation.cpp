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

  // Create a breakpoint for our simulation run. This observes the population
  // after each iteration and decides if we have reached our termination
  // conditions. If so, the breakpoint should set the elite reference to the
  // candidate that has satisfied the conditions. 
  // TODO: Multiple elites?
  auto breakpoint = [](const Population& pop, Candidate& elite) {
    auto match = std::find_if(pop.begin(), pop.end(), [](const Candidate& c){
      return (pr::fitness(c) == 0.0);
    });
    
    if (match != std::end(pop)) {
      elite = *match;
      return true;
    }

    return false;
  };

  sim.registerObserver((pr::EventFlags)0, [](const Population& pop) {
    std::cout << pr::progeny(pop[0]) << std::endl;
  });
  sim.evolve(10, 2, breakpoint);
}
