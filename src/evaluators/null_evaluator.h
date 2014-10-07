#ifndef NULL_EVALUATOR_H
#define NULL_EVALUATOR_H

#include "../core/evaluator.h"

template <typename Progeny>
class NullEvaluator : public Evaluator<Progeny> {

  public:
    double fitness(Progeny&& pr) {
      return 0.0; 
    }
};

#endif
