#ifndef COMPETITIVE_EVALUATOR_H
#define COMPETITIVE_EVALUATOR_H

#include <functional>

#include "../core/evaluator.h"
#include "../core/candidate.h"

namespace pr {

  template <typename CType, size_t N>
  class CompetitiveEvaluator : public Evaluator<CType> {

    using Population = pr::Population<CType>;

    public: 
      using SubPopulation = std::array<std::reference_wrapper<CType>, N>;
      using Compete = std::function<void(SubPopulation sp)>;

    public:
      CompetitiveEvaluator(Compete c) : Evaluator<CType>(), 
        m_compete(std::forward<Compete>(c)) {}

      virtual void evaluate(Population& pop) {
        #pragma omp parallel for
        for (size_t i = 0; i < pop.size() - (N - 1); i = i + N){
          SubPopulation sp;
          auto start = pop.begin() + i;
          auto end = start + N;
          std::transform(start, end, sp.begin(), [](CType cn){
              return std::ref(cn);
          });
          m_compete(sp);
        }
      }

    private:
      Compete m_compete;

  };
}

#endif
