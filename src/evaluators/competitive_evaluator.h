#ifndef COMPETITIVE_EVALUATOR_H
#define COMPETITIVE_EVALUATOR_H

#include <functional>

#include "../core/evaluator.h"
#include "../core/candidate.h"
#include "../core/type_traits.h"

namespace pr {

  template <typename CType, size_t N>
  class CompetitiveEvaluator : public Evaluator<CType> {

    using Population = pr::Population<CType>;
    using PopItr = typename Population::iterator;
    using Compete = typename std::function<void(PopItr, PopItr)>;

    private:
      
    public:
      CompetitiveEvaluator(Compete&& c) : Evaluator<CType>(), 
        m_compete(std::forward<Compete>(c)) {}

      virtual void evaluate(Population& pop) {
        #pragma omp parallel for
        for (size_t i = 0; i < pop.size() - (N - 1); i = i + N){
          auto start = pop.begin() + i;
          auto end = start + N;
          m_compete(start, end);
        }
      }

    private:
      Compete m_compete;

  };
}

#endif
