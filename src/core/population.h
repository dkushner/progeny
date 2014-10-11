#ifndef POPULATION_H
#define POPULATION_H

#include <vector>
#include <type_traits>
#include <algorithm>

#include "candidate.h"
#include "type_traits.h"

namespace pr {

  template <typename CType, class Enable = void>
  class Population;

  template <typename CType>
  class Population<
    CType,
    typename std::enable_if<is_specialization_of<Candidate, CType>::value>::type
  > : public std::vector<CType> {

    // Kind of redundant given our inheritance from std::vector
    // is transparent and value_type covers the same thing, but
    // this is consistent with other classes' type exposures.
    public:
      typedef CType CandidateType;

    public:
      Population(std::initializer_list<typename CType::BaseType> list) : 
        std::vector<CType>(list.begin(), list.end()) {}
  };
}

#endif
