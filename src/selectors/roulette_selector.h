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
    using FitnessType = typename Candidate::FitnessType;

    public:
      void select(Population& pop, int count, bool natural = true) {
        
        std::default_random_engine gen;
        std::vector<FitnessType> weights(pop.size());

        // If necessary, re-normalize population.
        if (!natural) {
          FitnessType max_fit{};
          #pragma omp parallel
          {
            #pragma omp for reduction(max : max_fit)
            for (int x = 0; x < pop.size(); ++x) {
              if (pr::fitness(pop[x]) > max_fit) {
                max_fit = pr::fitness(pop[x]);
              }
            }
            #pragma omp flush(max_fit)

            #pragma omp for
            for (int i = 0; i < pop.size(); ++i) {
              weights[i] = (max_fit + 1) - pr::fitness(pop[i]);
              pop[i].alive = false;
            }
          }
        } else {
          #pragma omp parallel for
          for (int i = 0; i < pop.size(); ++i) {
            weights[i] = pr::fitness(pop[i]);
            pop[i].alive = false;
          }
        }

        for (int i = 0; i < count; ++i){
          std::discrete_distribution<> dist(weights.begin(), weights.end());
          int idx = dist(gen);
          weights[idx] = 0.0;
          pop[idx].alive = true;
        }
      }
  };
}

#endif
