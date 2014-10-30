#ifndef EVALUATOR_H
#define EVALUATOR_H

#include <type_traits>

#include "type_traits.h"

namespace pr {

  template <typename CType, class Enable = void>
  class Evaluator;

  //! Base class of fitness evaluators. 
  /*!
  *  This class represents a basic implementation of a fitness
  *  evaluator for an evolutionary algorithm. 
  *  \tparam CType Candidate type that this class's descendants
  *  will evaluate.
  */
  template <typename CType>
  class Evaluator<
    CType,
    typename std::enable_if<is_specialization_of<Candidate, CType>::value>::type
  > {

    public:
      using Candidate = CType;
      using Population = pr::Population<CType>;
      using FitnessType = typename CType::FitnessType;
      using BaseType = typename CType::BaseType;

    public:
      Evaluator() = default;
      ~Evaluator() = default;

      //! Evaluates the given population.
      /*!
      *  This function is overridden (NOT hidden) for specialized 
      *  implementations. At the evaluation stage of the evolution process, 
      *  each member of the population, will be passed by reference to this 
      *  function. The function should retrieve the base-type, evaluate its 
      *  fitness,then assign that fitness to the candidate.
      *  \param mbr The population member to be evaluated.
      */
      void evaluate(Population& pop) {}
  };

}

#endif
