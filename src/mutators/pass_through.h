#ifndef PASS_THROUGH_H
#define PASS_THROUGH_H

#include <vector>

#include "../core/mutator.h"

namespace pr {
  //! Pass through mutation operator.
  /*!
  *  This mutator simply forwards the population as it is.
  */
  template <typename CType>
  class PassThrough : public Mutator<CType> {

    public:
      using Candidate = typename Mutator<CType>::Candidate;
      using Population = typename Mutator<CType>::Population;

    public:
      PassThrough() : Mutator<CType>() {};

      void mutate(Population& pop) {}
  };
}

#endif
