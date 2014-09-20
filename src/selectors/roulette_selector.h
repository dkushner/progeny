#ifndef ROULETTE_SELECTOR_H
#define ROULETTE_SELECTOR_H

#include <random>
#include <omp.h>
#include <iostream>

#include "../core/selector.h"

template <typename Progeny>
class RouletteSelector : Selector<Progeny, RouletteSelector<Progeny>> {

  using Population = std::vector<Progeny>;
  using RankedPopulation = std::vector<std::pair<Progeny, double>>;

  public:
    Population select(RankedPopulation&& rpop, unsigned int count) {
      double fit_max = 0.0;
      unsigned int selected = 0;
      
      Population pop(count);

      std::default_random_engine gen;
      std::uniform_real_distribution<double> dist(0.0, 1.0);

      #pragma omp parallel default(shared)
      {
        #pragma omp for reduction(+:fit_max)
        for (int x = 0; x < rpop.size(); x++) {
          fit_max = fit_max + std::get<1>(rpop[x]);
        }
        #pragma flush(fit_max)
        
        while (selected < count) {
          #pragma omp for 
          for (int x = 0; x < rpop.size(); x++ ) {
            #pragma omp flush(selected)
  
            if (dist(gen) < (std::get<1>(rpop[x]) / fit_max)) {

              #pragma omp critical
              { 
                if (selected < count) {
                  pop[selected] = std::get<0>(rpop[x]);
                  selected = selected + 1;
                }
              }
            }
          }
        }
      }
      return pop;
    }
};

#endif
