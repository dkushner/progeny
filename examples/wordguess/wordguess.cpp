#include <core/progeny.h>
#include <algorithm>
#include <core/simulation.h>
#include <evaluators/string_evaluator.h>
#include <generators/string_generator.h>
#include <selectors/roulette_selector.h>
#include <mutators/string_transition.h>
#include <mutators/crossover.h>
#include <mutators/pass_through.h>

int main(int argc, const char* argv[]) {

  StringGenerator sg(6);
  StringEvaluator se("target");
  RouletteSelector<std::string> rs;

  const std::string VALID = "abcdefghijklmnopqrstuvwxyz";

  auto pipeline = PassThrough<std::string>()
    >> Crossover<2, std::string>() 
    >> StringTransition(VALID, 0.2);

  using Candidate = std::pair<std::string, double>;
  using RankedPopulation = std::vector<Candidate>;
  
  Simulation<std::string> sim(sg, se, rs, pipeline);
  sim.evolve(20, 2, [](RankedPopulation& rpop, std::string& elite) {
    auto max = std::find_if(rpop.begin(), rpop.end(), [](Candidate& can) {
      if (std::get<1>(can) == 0.0) {
        return true;
      }
      return false;
    });

    if (max != std::end(rpop)) {
      elite = std::get<0>(*max);
      return true;
    } else {
      return false;
    }
  });
}
