#ifndef NULL_EVALUATOR_H
#define NULL_EVALUATOR_H

#include <omp.h>

#include "../core/evaluator.h"
#include "../core/candidate.h"

namespace pr {

  template <typename CType>
  class NullEvaluator : public Evaluator<CType> {

    using FitnessType = typename CType::FitnessType;

    public:
      void evaluate(Population<CType>& pop) {
        #pragma omp parallel for
        for (size_t i = 0; i < pop.size(); i++) {
          pr::fitness(pop[i]) = FitnessType{};
        }
      }
  };

}

#endif
