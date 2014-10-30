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
      using Initializer = std::function<typename CType::BaseType(void)>;
  
    public:
      Generator(Initializer i) : m_initializer(std::forward<Initializer>(i)) {}
      ~Generator() = default; 

      void generate(Population& seed, size_t size) {}

    protected:
      Initializer m_initializer; 
  };
}

#endif
