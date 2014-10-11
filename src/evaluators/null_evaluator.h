#ifndef NULL_EVALUATOR_H
#define NULL_EVALUATOR_H

#include "../core/evaluator.h"
#include "../core/candidate.h"

namespace pr {

  template <typename CType>
  class NullEvaluator : public Evaluator<CType> {

    using FitType = typename CType::FitnessType;

    public:
      typedef CType CandidateType;

    public:
      void evaluate(CType& can) {
        pr::fitness(can) = FitType{};
      }
  };

}

#endif
