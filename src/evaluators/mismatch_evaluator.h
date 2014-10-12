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
      typedef CType Candidate;

    public:
      MismatchEvaluator(CType proto) : m_target(proto) {};

      void evaluate(CType& cnd) const {
        FitType error{};

        const BType& proto = pr::progeny(m_target);
        BType& sample = pr::progeny(cnd);

        size_t proto_size = proto.size();
        size_t sample_size = sample.size();

        // The use of #size() here is iffy. For strings, this 
        // returns only the number of bytes in the string, whereas
        // for vectors it returns the number of elements. For wide
        // character encodings, this may not be reliable.
        error += std::abs(proto_size - sample_size);

        size_t min_len = std::min(proto_size, sample_size);
        for (int i = 0; i < min_len; i++) {
          if (!(proto[i] == sample[i])) {
            error += 1;
          }
        }

        pr::fitness(cnd) = error;
      }

    protected:
      typedef typename CType::BaseType BType;
      typedef typename CType::FitnessType FitType;

    private:
      const CType m_target;

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
    typedef CType CandidateType;

    public: 
      // Yes, the prototype is a candidate. I'm hoping that, 
      // because candidates should be trivially constructible
      // from a BaseType parameter, that the user doesn't actually
      // have to specify/create a CType instance. 
      //
      // There's also the fact that some evaluators may rely 
      // on the last iteration's fitness scores in making decisions 
      // (i.e. competitive evaluators.
      MismatchEvaluator(CType proto) : m_target(proto) {};

      void evaluate(CType& cnd) const {
        // Since its arithmetic, should be zero-initialized.
        FitType error{};
        Match<Size - 1>::match(m_target, cnd, error);
        pr::fitness(cnd) = error;
      }
    
    protected:
      typedef typename CType::BaseType BType;
      typedef typename CType::FitnessType FitType;
      static const size_t Size = std::tuple_size<BType>::value;

    private:
      const CType m_target;
  };

  template <unsigned int X>
  struct Match {

    template <typename CType, typename FitType>
    static void match(const CType& proto, CType& can, FitType& errors) {
      if (!(std::get<X>(pr::progeny(proto)) == std::get<X>(pr::progeny(can)))) {
        errors++;
      }
      Match<X - 1>::match(proto, can, errors);
    }

  };

  template <>
  struct Match<0> {

    template <typename CType, typename FitType>
    static void match(const CType& proto, CType& can, FitType& errors) {
      if (!(std::get<0>(pr::progeny(proto)) == std::get<0>(pr::progeny(can)))) {
        errors++;
      }
    }

  };
}

#endif 
