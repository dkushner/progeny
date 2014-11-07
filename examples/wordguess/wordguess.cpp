
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <boost/program_options.hpp>

#include <core/simulation.h>
#include <evaluators/mismatch_evaluator.h>
#include <selectors/roulette_selector.h>
#include <mutators/crossover.h>
#include <generators/fill_generator.h>
#include <mutators/pass_through.h>

namespace po = boost::program_options;

int main(int argc, char** argv) {
  std::string target;
  unsigned int size;
  unsigned int seed;


  po::options_description desc("Recognized options");
  desc.add_options()
    ("help", "Print this help message. Obviously.")
    ("target", po::value<std::string>(&target)->default_value("target"), 
      "Target string to evolve towards.")
    ("size", po::value<unsigned int>(&size)->default_value(100), 
      "Population size to use in the evolution.")
    ("seed", po::value<unsigned int>(&seed), "Optional seed for the RNG.");
  
  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(desc).run(), vm);
  po::notify(vm);

  // Set up RNG.
  std::random_device rd;
  std::mt19937 mt(vm.count("seed") ? seed : rd());
  std::uniform_int_distribution<int> dist(0, 25);
  const char valid[] = "abcdefghijklmnopqrstuvwxyz";

  // Aliases for cleanliness.
  using Candidate = pr::Candidate<std::string, double>;
  using Population = pr::Population<Candidate>;
  using Data = pr::Simulation<Candidate>::Data;

  // Construct Generator
  pr::FillGenerator<Candidate> fg([&]{
    std::string str(target.size(), 0);
    std::generate(str.begin(), str.end(), [&]{
      return valid[dist(mt)];
    });
    return str;
  });

  // Construct Evaluator
  pr::MismatchEvaluator<Candidate> mev(target);

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
    std::cout << "Evolution took " << data.generation
      << " iterations and " << data.elapsedTime
      << " seconds.\xd";
  });

  // Run the actual simulation.
  sim.evolve(size, 1, breakpoint);
  std::cout << std::endl;

  // Run the brute-force attempt.
  std::string brute(target.size(), 0);
  size_t iterations = 0;
  auto start = std::chrono::high_resolution_clock::now();
  do {
    std::generate(brute.begin(), brute.end(), [&]{
      return valid[dist(mt)];
    });
    ++iterations;
    
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::high_resolution_clock::now() - start
    ).count();
    std::cout << "Brute force took " << iterations 
      << " iterations and " << elapsed
      << " seconds.\xd";
  } while (brute != target);
  std::cout << std::endl;
}
