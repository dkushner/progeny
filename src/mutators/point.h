#ifndef POINT_H
#define POINT_H

#include "../core/mutator.h"
#include "../core/type_traits.h"

namespace pr {

  //! Point mutation operator.
  /*!
   * This mutator modifies a single point in the candidate based on a 
   * defined set, its associated probabilities and a binary selection
   * probability.
   */
  template <typename CType, class Enable = void>
  class Point;

  //! Point mutation operator overloaded for statically sized containers.
  template <typename CType>
  class Point<
    CType,
    typename std::enable_if<is_static_container<CType>::value>::type
  > : public Mutator<CType> {

    public:
      using Candidate = typename Mutator<CType>::Candidate;
      using Population = typename Mutator<CType>::Population;

    public:
      Point(){}

  };

  //! Point mutation operator overloaded for dynamically sized containers.
  template <typename CType>
  class Point<
    CType,
    typename std::enable_if<
      !is_static_container<typename CType::BaseType>::value &&
      has_value_type<typename CType::BaseType>::value
    >::type
  > : public Mutator<CType> {

    public:
      Point(){}

  };

}

#endif
