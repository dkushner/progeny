#ifndef SELECTOR_H
#define SELECTOR_H

#include "population.h"

namespace pr {

  //! Base candidate selection strategy.
  template <typename CType, class Enable = void>
  class Selector;

  template <typename CType>
  class Selector<
    CType,
    typename std::enable_if<is_specialization_of<Candidate, CType>::value>::type
  > {

    public:
      using Candidate = CType;
      using Population = pr::Population<CType>;

    public:
      Selector() {};
      ~Selector() {};

      Population select(Population& pop, int count, bool natural = true) {}
  };
}

#endif
