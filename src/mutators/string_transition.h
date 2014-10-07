#ifndef TRANSITION_H 
#define TRANSITION_H 

#include "../core/mutator.h"
#include "../core/type_traits.h"

class StringTransition : public Mutator<std::string> {

  public:
    StringTransition(std::string v, double p) : 
      Mutator<std::string>(), m_valid(v), m_probability(p) {};

    Population& operator()(Population&& pop) {
      std::default_random_engine gen;
      std::uniform_int_distribution<int> dist(0, m_valid.length() - 1);

      #pragma omp parallel for 
      for (int i = 0; i < pop.size(); i++) {
        std::transform(pop[i].begin(), pop[i].end(), pop[i].begin(), [&](char c) {
          double roll = gen();
          if (roll < m_probability) {
            return m_valid[dist(gen)];
          } else {
            return c;
          }
        });
      }
    }

  private:
    const std::string m_valid;
    const double m_probability;
};

#endif
