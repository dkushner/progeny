#include <iomanip>
#include <iostream>
#include <algorithm>
#include <core/simulation.h>
#include <evaluators/mismatch_evaluator.h>
#include <selectors/roulette_selector.h>
#include <mutators/crossover.h>
#include <generators/fill_generator.h>
#include <mutators/pass_through.h>

int main(int argc, const char* argv[]) {

  using Candidate = pr::Candidate<std::string, double>;
  using Population = pr::Population<Candidate>;
  using Data = pr::Simulation<Candidate>::Data;

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

  // Register an observer function that watches the population.
  sim.addObserver([](const Data& data) {
    std::cout << std::setw(10) << data.generation 
    << std::setw(10) << data.elapsedTime
    << std::setw(10) << data.meanFitness
    << '\xd';
  });

  sim.evolve(50, 10, breakpoint);
}
