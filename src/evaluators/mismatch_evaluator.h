#ifndef MISMATCH_EVALUATOR_H 
#define MISMATCH_EVALUATOR_H

#include <algorithm>

#include "../core/evaluator.h"
#include "../core/candidate.h"
#include "../core/type_traits.h"

namespace pr {

  template <unsigned int X>
  struct Match;

  //! Evaluator that assigns error points for non-matching elements.
  /*!
   *  This evaluator assigns error points to each candidate instance
   *  based on the following schedule:
   *      
   *      (a) +1 error point for every element in the progeny
   *         that for which progeny[i] == prototype[i] returns false.
   *
   *      (b) +N error points where N is the absolute value of the 
   *         length difference between progeny and prototype.
   *
   *  If the progeny type is a statically-sized container, schedule
   *  (b) is not assessed.
  */
  template <typename CType, class Enable = void>
  class MismatchEvaluator;

  //! Specialization for variable-size containers.
  template <typename CType>
  class MismatchEvaluator<
    CType,
    typename std::enable_if<
      !is_static_container<typename CType::BaseType>::value &&
      has_value_type<typename CType::BaseType>::value
    >::type
  > : public Evaluator<CType> {

    public:
      using typename Evaluator<CType>::Candidate;
      using typename Evaluator<CType>::Population;
      using BaseType = typename Candidate::BaseType;

    public:
      MismatchEvaluator(const BaseType proto) : m_target(proto) {};

      void evaluate(Population& pop) const {

        #pragma omp parallel for
        for (int i = 0; i < pop.size(); i++) {

          CType& cnd = pop[i];
          FitType error{};

          const BaseType& proto = m_target;
          BaseType& sample = pr::progeny(cnd);

          int proto_size = proto.size();
          int sample_size = sample.size();

          // The use of #size() here is iffy. For strings, this 
          // returns only the number of bytes in the string, whereas
          // for vectors it returns the number of elements. For wide
          // character encodings, this may not be reliable.
          error += std::abs(proto_size - sample_size);

          int min_len = std::min(proto_size, sample_size);
          for (int i = 0; i < min_len; i++) {
            if (!(proto[i] == sample[i])) {
              error += 1;
            }
          }
          pr::fitness(cnd) = error;
        }

      }

    protected:
      typedef typename CType::FitnessType FitType;

    private:
      const BaseType m_target;

  };


  //! Specialization for statically-sized containers.
  template <typename CType>
  class MismatchEvaluator<
    CType,
    typename std::enable_if<
      is_static_container<typename CType::BaseType>::value &&
      is_piecewise_comparable<typename CType::BaseType>::type::value
    >::type
  > : public Evaluator<CType> {

    // Abbreviated dependent typedefs for utility.
    
    public:
      using typename Evaluator<CType>::Candidate;
      using typename Evaluator<CType>::Population;
      using BaseType = typename Candidate::BaseType;

    public: 
      MismatchEvaluator(BaseType proto) : m_target(proto) {};

      void evaluate(Population& pop) const {
        #pragma omp parallel for
        for (size_t i = 0; i < pop.size(); i++) {

          // Since its arithmetic, should be zero-initialized.
          FitType error{};
          Match<Size - 1>::match(m_target, pop[i], error);
          pr::fitness(pop[i]) = error;
        }
      }
    
    protected:
      typedef typename CType::FitnessType FitType;
      static const size_t Size = std::tuple_size<BaseType>::value;

    private:
      const BaseType m_target;
  };

  template <unsigned int X>
  struct Match {

    template <typename Base, typename Fit>
    static void match(const Base& proto, Candidate<Base, Fit>& can, Fit& errors) {
      if (!(std::get<X>(proto) == std::get<X>(pr::progeny(can)))) {
        errors++;
      }
      Match<X - 1>::match(proto, can, errors);
    }

  };

  template <>
  struct Match<0> {

    template <typename Base, typename Fit>
    static void match(const Base& proto, Candidate<Base, Fit>& can, Fit& errors) {
      if (!(std::get<0>(proto) == std::get<0>(pr::progeny(can)))) {
        errors++;
      }
    }

  };
}

#endif 
