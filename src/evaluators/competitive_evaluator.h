#ifndef COMPETITIVE_EVALUATOR_H
#define COMPETITIVE_EVALUATOR_H

#include <functional>

#include "../core/evaluator.h"
#include "../core/candidate.h"

namespace pr {

  template <typename CType, size_t N>
  class CompetitiveEvaluator : public Evaluator<CType> {
    using typename Evaluator<CType>::Population;

    using PopItr = typename Evaluator<CType>::Population::iterator;
    using Compete = std::function<void(PopItr, PopItr)>;

    public:
      CompetitiveEvaluator(Compete c) : Evaluator<CType>(), 
        m_compete(std::forward<Compete>(c)) {}

      void evaluate(Population& pop) const {
        #pragma omp parallel for
        for (size_t i = 0; i < pop.size() - (N - 1); i = i + N){
          auto first_itr = pop.begin() + i;
          auto last_itr = first_itr + N;

          m_compete(first_itr, last_itr);
        }
      }

    private:
      Compete m_compete;

  };
}

#endif
