#ifndef ROULETTE_SELECTOR_H
#define ROULETTE_SELECTOR_H

#include <random>
#include <omp.h>
#include <iostream>

#include "../core/selector.h"

template <typename Progeny>
class RouletteSelector : public Selector<Progeny> {

  using Population = std::vector<Progeny>;
  using Candidate = std::pair<Progeny, double>;
  using RankedPopulation = std::vector<std::pair<Progeny, double>>;

  public:
    Population select(RankedPopulation&& rpop, int count, bool natural = true) {
      
      Population pop(count);
      unsigned int selected = 0;

      std::vector<double> weights;
      auto inserter = std::back_inserter(weights);
      auto selector = [](const Candidate& p) {
        return std::get<1>(p);
      };

      // Pluck the weights from the ranked population.
      std::transform(rpop.begin(), rpop.end(), inserter, selector);

      // If necessary, re-normalize population.
      if (!natural) {
        double max_fit = 0.0;
        #pragma omp parallel
        {
          #pragma omp for reduction(max:max_fit)
          for (int x = 0; x < weights.size(); x++) {
            max_fit = weights[x];
          }
          #pragma omp flush(max_fit)
          
          #pragma omp for 
          for (int i = 0; i < weights.size(); i++) {
            weights[i] = (max_fit + 1) - weights[i];
          }
        }
      } 

      std::default_random_engine gen;
      std::discrete_distribution<> dist(weights.begin(), weights.end());

      #pragma omp parallel for 
      for (int i = 0; i < pop.size(); i++) {
        pop[i] = std::get<0>(rpop[dist(gen)]);
      }
      return pop;
    }

    Population select(RankedPopulation& rpop, int count, bool natural = true) {
      return select(std::move(rpop), count, natural);
    }
};

#endif
