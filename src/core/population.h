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

    public:
      Population() : std::vector<CType>() {}
      Population(size_t size) : std::vector<CType>(size) {}
      Population(std::initializer_list<typename CType::BaseType> list) : 
        std::vector<CType>(list.begin(), list.end()) {}
      Population(std::initializer_list<CType> list) :
        std::vector<CType>(list) {}

  };
}

#endif
