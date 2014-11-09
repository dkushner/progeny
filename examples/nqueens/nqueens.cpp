#include <iomanip>
#include <iostream>
#include <algorithm>
#include <core/simulation.h>
#include <evaluators/competitive_evaluator.h>
#include <selectors/roulette_selector.h>
#include <mutators/crossover.h>
#include <observers/terminal_observer.h>
#include <generators/fill_generator.h>
#include <mutators/pass_through.h>

#ifndef QUEENS
#define QUEENS 8
#endif

int main(int argc, const char* argv[]) {

  using Candidate = pr::Candidate<std::array<int, QUEENS>, int>;
  using Population = pr::Population<Candidate>;
  using PopItr = Population::iterator;

  std::cout << QUEENS << std::endl;

  // Construct Generator
  pr::FillGenerator<Candidate> fg([]{
    std::array<int, QUEENS> board;
    std::generate(board.begin(), board.end(), []{
      return rand() % QUEENS;
    });
    return board;
  });

  // Construct Evaluator
  pr::CompetitiveEvaluator<Candidate, 1> cev([&](PopItr s, PopItr e){
    auto board = pr::progeny(*s);

    int attacks = 0;
    for (int col = 0; col < QUEENS; col++) {
      int pos = board[col];
      for (int adv = col + 1; adv < QUEENS; adv++) {
        if (board[adv] == pos) {
          ++attacks;
        }
        
        if (board[adv] == pos + (adv - col)) {
          ++attacks;
        }

        if (board[adv] == pos - (adv - col)) {
          ++attacks;
        }
      }
    }
    pr::fitness(*s) = attacks;
  });

  // Construct Selector
  pr::RouletteSelector<Candidate> rs;

  // Construct Mutator
  auto mut = pr::Crossover<Candidate>(2) >> pr::PassThrough<Candidate>();

  // Finally, compose the simulator instance.
  auto sim = pr::Simulation<Candidate>::build(fg, cev, rs, mut);

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
  pr::TerminalObserver<Candidate> tobs;
  tobs.bind(sim);

  Candidate solution = sim.evolve(50, 10, breakpoint);
  std::cout << std::endl;
  for (auto a : pr::progeny(solution)) {
    std::cout << a << " ";
  }
}
