#ifndef PASS_THROUGH_H
#define PASS_THROUGH_H

#include <vector>

#include "../core/mutator.h"

//! Pass through mutation operator.
/*!
 *  This mutator simply forwards the population as it is.
 */
template <typename Progeny>
class PassThrough : public Mutator<Progeny> {

  using Population = std::vector<Progeny>;

  public:
    PassThrough() : Mutator<Progeny>() {};

    Population& operator()(Population&& pop) {
      return pop;
    }
    Population& operator()(Population& pop) {
      return operator()(std::move(pop));
    }
};

#endif
