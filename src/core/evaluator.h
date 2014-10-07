#ifndef EVALUATOR_H
#define EVALUATOR_H

#include <iostream>
#include <vector>
#include <omp.h>
#include <functional>
#include <algorithm>

namespace pr {

  //! Base class of fitness evaluators. 
  /*!
  *  This class represents a basic implementation of a fitness
  *  evaluator for an evolutionary algorithm. 
  *  \tparam CType Candidate type that this class will evaluate.
  */
  template <typename CType>
  class Evaluator {

    typedef CType::BaseType BaseType;

    public:
      Evaluator() {}
      virtual ~Evaluator() {}

      //! Evaluates the given population.
      /*!
      *  This function is overridden for specialized implementations.
      *  At the evaluation stage of the evolution process, each member
      *  of the population, will be passed by reference to this function.
      *  The function should retrieve the base-type, evaluate its fitness,
      *  then assign that fitness to the candidate.
      *  \param mbr The population member to be evaluated.
      */
      virtual void evaluate(const CType& mbr) const {}
  };

}

#endif
