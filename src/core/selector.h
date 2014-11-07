#ifndef SELECTOR_H
#define SELECTOR_H

#include "population.h"

namespace pr {

  //! Base candidate selection strategy.
  template <typename CType>
  class Selector {

    static_assert(is_specialization_of<Candidate, CType>::value, 
        "Template parameter must specialize Candidate.");

    public:
      Selector() = default;
      ~Selector() = default;

      virtual void select(Population<CType>& pop, int count) = 0;
      virtual bool isNatural() { return true; }
  };
}

#endif
