#ifndef ROULETTE_SELECTOR_H
#define ROULETTE_SELECTOR_H

#include <random>
#include <omp.h>
#include <iostream>

#include "../core/selector.h"

namespace pr {

  template <typename CType>
  class RouletteSelector : public Selector<CType> {

    using typename Selector<CType>::Candidate;
    using typename Selector<CType>::Population;

    public:
      void select(Population& pop, int count, bool natural = true) {
        
        Population sub_pop(count);
        unsigned int selected = 0;

        std::vector<double> weights;
        auto inserter = std::back_inserter(weights);
        auto selector = [](const Candidate& p) {
          return pr::fitness(p);
        };

        // Pluck the weights from the ranked population.
        std::transform(pop.begin(), pop.end(), inserter, selector);

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
        for (int i = 0; i < sub_pop.size(); i++) {
          sub_pop[i] = pop[dist(gen)];
        }
        pop = sub_pop;
      }
  };
}

#endif
