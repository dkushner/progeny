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
  template <typename CType>
  class Generator {

    static_assert(is_specialization_of<Candidate, CType>::value, 
        "Template parameter must specialize Candidate.");
    
    public: 
      using Candidate = CType;
      using Population = pr::Population<CType>;
  
    public:
      Generator() = default;
      virtual ~Generator() = default; 
      virtual void generate(Population&) = 0;
  };
}

#endif
