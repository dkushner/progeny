#ifndef MUTATOR_H
#define MUTATOR_H

#include <utility>
#include <type_traits>
#include <vector>
#include <functional>
#include <tuple> 

#include "candidate.h"
#include "type_traits.h"

namespace pr {

  //! Base mutation operator.
  /*!
  *  This base class may be extended to provide customized mutation 
  *  operators for an evolution pipeline.
  */
  template <typename CType, class Enable = void>
  class Mutator;

  template <typename CType>
  class Mutator<
    CType,
    typename std::enable_if<is_specialization_of<Candidate, CType>::value>::type
  > {

    public:
      typedef CType CandidateType;
    
    public:
      Mutator() = default; 
      ~Mutator() = default; 

      /*!
      *  Mutates the provided population in place. 
      *  \param p The population to operate on.
      *  \returns The population, after mutation.
      */
      void operator()(Population& p) {}
  };

  //! Encapsulated series of evolutionary operators.
  /*!
  *  This class encapsulates a series of evolutionary operators 
  *  and provides syntax sugar for easily and fluently constructing 
  *  evolutionary pipelines from constituent operations.
  */
  template <typename CType, typename LMType, RMType>
  class Pipeline : public Mutator<CType> {

    friend 

    private:
      Pipeline(Mutator<CType> i, Mutator<CType> o) :
        m_inner(std::forward<Mutator<Progeny>>(i)),
        m_outer(std::forward<Mutator<Progeny>>(o)) {};

      /*!
      *  \tparam O Type of the evolutionary operator to be appended to the
      *  pipeline. This is deduced at compile-time automatically.
      *  \param otr The evolutionary operator instance to append to the 
      *  pipeline.
      *  \returns The concatenated evolutionary pipeline.
      */
      Pipeline& operator>>(Pipeline pr) const {
        return Pipeline(*this, pr);
      }

      /*!
      *  Processes the given population using this evolutionary pipeline.
      *
      *  \tparam PopType The population type of concern. This is deduced
      *  at compile-time automatically.
      *  \param p The population to process.
      *  \returns The processed population.
      */
      Population& operator()(Population&& p) {
        return m_outer(m_inner(p));
      }

      Population& operator()(Population& p) {
        return m_outer(m_inner(std::move(p)));
      }

    protected:
      Mutator<Progeny> m_inner;
      Mutator<Progeny> m_outer;
  };

  template <typename CType, typename LMType, typename RMType>
  typename std::enable_if<
    std::is_base_of<Mutator<CType>, MType>::value, 
    Pipeline<CType>
  >::type operator>>(const Mutator<Progeny>& l, const MType& r) {
    return Pipeline<Progeny>(l, r);
  }
}

#endif
