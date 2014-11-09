#ifndef EVALUATOR_H
#define EVALUATOR_H

#include <type_traits>

#include "type_traits.h"

namespace pr {

  //! Base class of fitness evaluators. 
  /*!
  *  This class represents a basic implementation of a fitness
  *  evaluator for an evolutionary algorithm. 
  *  \tparam CType Candidate type that this class's descendants
  *  will evaluate.
  */
  template <typename CType>
  class Evaluator {

    static_assert(is_specialization_of<Candidate, CType>::value,
        "Template parameter must specialize Candidate.");

    public: 
      using Population = pr::Population<CType>;

    public:
      Evaluator() = default;
      virtual ~Evaluator() = default;

      //! Evaluates the given population.
      /*!
      *  This function is overridden (NOT hidden) for specialized 
      *  implementations. At the evaluation stage of the evolution process, 
      *  each member of the population, will be passed by reference to this 
      *  function. The function should retrieve the base-type, evaluate its 
      *  fitness,then assign that fitness to the candidate.
      *  \param mbr The population member to be evaluated.
      */
      virtual void evaluate(Population&) = 0;
      virtual bool isNatural() { return true; }
  };

}

#endif
