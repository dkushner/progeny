#ifndef GENERATOR_H
#define GENERATOR_H

#include <functional>
#include <vector>
#include <random>

#include "population.h"

namespace pr {

  //! Abstract base class of population generators.
  /*!
  *  This abstract base class may be extended to provide specialized 
  *  population members for any given population type.
  */
  template <typename CType, class Enable = void>
  class Generator;

  template <typename CType>
  class Generator<
    CType,
    typename std::enable_if<is_specialization_of<Candidate, CType>::value>::type
  > {

    public: 
      using Candidate = CType;
      using Population = pr::Population<CType>;
  
    public:
      Generator() = default;
      ~Generator() = default; 

      void generate(Population& seed, size_t size) {}
  };
}

#endif
