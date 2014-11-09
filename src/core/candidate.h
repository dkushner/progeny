#ifndef CANDIDATE_H 
#define CANDIDATE_H 

#include <utility>
#include <type_traits>

namespace pr {

  //! Encapsulates the basic population member type.
  /*!
  *  \tparam BType Base type of population member.
  *  \tparam FitType Type of the fitness quantity.
  */
  template <typename Base, typename Fitness>
  class Candidate : public std::pair<Base, Fitness> {
    
    static_assert(std::is_arithmetic<Fitness>::value, 
        "Fitness type must be arithmetic.");

    public:
      using BaseType = Base;
      using FitnessType = Fitness;

    public:
      Candidate() : std::pair<Base, Fitness>(), alive(false) {}
      Candidate(Base b) : 
        std::pair<Base, Fitness>(b, FitnessType()) {}
      Candidate(Base b, Fitness f) : 
        std::pair<Base, Fitness>(b, f) {}

    public:
      bool alive = true;

  };

  template <typename BType, typename FitType>
  FitType& fitness(Candidate<BType, FitType>& cnd) {
    return std::get<1>(cnd);
  }

  template <typename BType, typename FitType>
  FitType&& fitness(Candidate<BType, FitType>&& cnd) {
    return std::move(std::get<1>(cnd));
  }

  template <typename BType, typename FitType>
  const FitType& fitness(const Candidate<BType, FitType>& cnd) {
    return std::get<1>(cnd);
  }

  template <typename BType, typename FitType>
  BType& progeny(Candidate<BType, FitType>& cnd) {
    return std::get<0>(cnd);
  }

  template <typename BType, typename FitType>
  BType&& progeny(Candidate<BType, FitType>&& cnd) {
    return std::move(std::get<0>(cnd));
  }

  template <typename BType, typename FitType>
  const BType& progeny(const Candidate<BType, FitType>& cnd) {
    return std::get<0>(cnd);
  }
    
}
#endif
