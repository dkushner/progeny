#ifndef MUTATOR_H
#define MUTATOR_H

#include <utility>
#include <type_traits>
#include <vector>
#include <functional>
#include <tuple> 

#include "candidate.h"
#include "population.h"
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
      using Candidate = CType;
      using Population = pr::Population<CType>;
    
    public:
      Mutator() = default; 
      ~Mutator() = default; 

      /*!
      *  Mutates the provided population in place. 
      *  \param p The population to operate on.
      *  \returns The population, after mutation.
      */
      void mutate(Population& p){}
  };

  //! Encapsulated series of evolutionary operators.
  /*!
  *  This class encapsulates a series of evolutionary operators 
  *  and provides syntax sugar for easily and fluently constructing 
  *  evolutionary pipelines from constituent operations.
  *  \tparam CType The candidate type that this pipeline operates on.
  *  \tparam OType The type of the outer mutator.
  *  \tparam IType The type of the inner mutator.
  */
  template <typename CType, typename IType, typename OType>
  class Pipeline : public Mutator<CType> {

    using Population = typename Mutator<CType>::Population;

    public:
      Pipeline(IType i, OType o) :
        m_inner(i), m_outer(o) {};

      void mutate(Population& p) {
        m_inner.mutate(p);
        m_outer.mutate(p);
      }

    protected:
      IType m_inner;
      OType m_outer;
  };

  template <
    typename CType, 
    template <typename...> class IType, 
    template <typename...> class OType
  >
  typename std::enable_if<
    std::is_base_of<Mutator<CType>, IType<CType>>::value &&
    std::is_base_of<Mutator<CType>, OType<CType>>::value,
    Pipeline<CType, IType<CType>, OType<CType>>
  >::type operator>>(const IType<CType>& i, const OType<CType>& o) {
    return std::move(Pipeline<CType, IType<CType>, OType<CType>>(i, o));
  }
}

#endif
