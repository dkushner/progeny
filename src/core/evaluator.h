#ifndef EVALUATOR_H
#define EVALUATOR_H

#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>

//! Base class of fitness evaluators. 
/*!
 *  This class represents a basic implementation of a fitness
 *  evaluator for an evolutionary algorithm. By default, it makes
 *  use of threaded concurrency to perform the independent 
 *  evaluations of each individual population member. 
 */
template <typename Progeny, typename Impl>
class Evaluator {

  using Population = std::vector<Progeny>;
  using RankedPopulation = std::vector<std::pair<Progeny, double>>;

  public:
    Evaluator() {}
    virtual ~Evaluator() {}

    //! Evaluates the given population.
    /*!
     *  \param p The population to be evaluated.
     *  \returns The ranked population. This maps each member of the 
     *  provided population to a pair consisting of that member and 
     *  the calculated fitness of that member.
     */
    RankedPopulation evaluate(Population p){
      RankedPopulation rpop(p.size());

      #pragma omp parallel for
      for (int i = 0; i < p.size(); i++) {
        rpop[i] = std::make_pair(p[i], fitness(std::forward<Progeny>(p[i])));
      }
      return rpop;
    }

  protected:
    //! Fitness function implemented by extending class. 
    /*!
     *  \param pr An individual progeny to be evaluated.
     *  \returns A double representing the progeny's arbitrary fitness
     *  value.
     */
    double fitness(Progeny&& pr) {
      return static_cast<Impl*>(this)->fitness(std::forward<Progeny>(pr));
    }
};

#endif
