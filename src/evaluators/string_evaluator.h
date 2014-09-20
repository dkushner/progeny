#ifndef STRING_EVALUATOR_H
#define STRING_EVALUATOR_H

#include <omp.h>
#include <string>
#include <iostream>

#include "../core/evaluator.h"

class StringEvaluator : public Evaluator<std::string, StringEvaluator> {

  PROGENY_TYPE(std::string);

  public: 
    StringEvaluator(Progeny target) : m_target(target) {};

    double fitness(Progeny&& pr) {
      if (pr.length() != m_target.length()) {
        return 0.0;
      }

      int checks = 0;
      for (int i = 0; i < m_target.length(); i++) {
        (pr[i] == m_target[i]) && ++checks;
      }

      return (double)checks / (double)m_target.length();
    }

  private:
    const std::string m_target;
};

#endif 
