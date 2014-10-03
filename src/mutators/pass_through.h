#ifndef PASS_THROUGH_H
#define PASS_THROUGH_H

#include "../core/mutator.h"

template <typename Progeny>
class PassThrough : public Mutator<Progeny, PassThrough<Progeny>> {

  using Population = std::vector<Progeny>;

  public:
    PassThrough() : Mutator<Progeny, PassThrough<Progeny>>() {};

    Population& operator()(Population&& pop) {
      return pop;
    }

    Population& operator()(Population& pop) {
      return operator()(std::move(pop));
    }
};

#endif
