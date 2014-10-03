#ifndef MUTATOR_H
#define MUTATOR_H

#include <functional>
#include <tuple>

#include "evolution_pipeline.h"

//! Base mutation operator.
/*!
 *  This base class may be extended to provide customized mutation 
 *  operators for an evolution pipeline.
 */
template <typename Progeny, typename Impl>
class Mutator {

  public:
    typedef std::vector<Progeny> Population;
  
  public:
    Mutator(const Mutator&) = default;
    Mutator& operator=(const Mutator&) = default;

    Mutator() = default;
    Mutator(Mutator&&) = default;
    Mutator& operator=(Mutator&&) = default;
    virtual ~Mutator() {};

    /*!
     *  Syntax sugar to facilitate adding this operator to an evolution
     *  pipeline.
     *  \tparam O Type of the operator to be appended. This is deduced
     *  at compile-time automatically.
     *  \param otr The operator to be appended.
     */
    template <typename O>
    auto operator>>(O&& otr) -> EvolutionPipeline<
      Mutator, decltype(std::forward<O>(otr))
    > {
      return EvolutionPipeline<
        Mutator, decltype(std::forward<O>(otr))
      >(*this, std::forward<O>(otr));
    }

    /*!
     *  Mutates the provided population in place. 
     *  \param p The population to operate on.
     *  \returns The population, after mutation.
     */
    Population& operator()(Population&& p) {
      return static_cast<Impl*>(this)->operator()(std::forward<Population>(p));
    }

    Population& operator()(Population& p) {
      return static_cast<Impl*>(this)->operator()(std::move(p));
    }
};

#endif
