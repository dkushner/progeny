#ifndef STRING_EVALUATOR_H
#define STRING_EVALUATOR_H

#include <omp.h>
#include <string>
#include <iostream>
#include <algorithm>

#include "../core/evaluator.h"
#include "../core/candidate.h"

namespace pr {
  template <typename FitType, class Enable = void>
  class StringEvaluator;

  //! Evaluator implementation for strings.
  /*!
  *  Evaluates the fitness of string type progeny given a particular
  *  target string. Differences in the string's length and specific
  *  character contents accrue error points that may either be 
  *  maximized or minimized by a selector.
  */
  template <typename FitType>
  class StringEvaluator<
    FitType,
    typename std::enable_if<std::is_arithmetic<FitType>::value>::type
  > : public Evaluator<Candidate<std::string, FitType> {

    public: 
      StringEvaluator(std::string target) : m_target(target) {};

      virtual void evaluate(const CType& pr) const {
        // Since its arithmetic, should be zero-initialized.
        FitType error();

        error += std::abs((FitType)m_target.length() - pr.length());

        int min_length = std::min(m_target.length(), pr.length());
        for (int i = 0; i < min_length; i++) {
          if(pr[i] != m_target[i]) {
            error += (FitType)1;
          }
        }
        return error;
      }

    private:
      typedef Candidate<std::string, FitType> CType;

    private:
      const std::string m_target;
  };
}

#endif 
