#ifndef FILL_GENERATOR_H 
#define FILL_GENERATOR_H

#include <string>
#include <algorithm>

#include "../core/generator.h"

namespace pr {

  template <typename CType, class Enable = void>
  class FillGenerator;

  template <typename CType>
  class FillGenerator<
    CType,
    typename std::enable_if<std::true_type::value>::type
  > : public Generator<CType> {

    public:
      using typename Generator<CType>::Candidate;
      using typename Generator<CType>::Population;
      using Initializer = std::function<typename CType::BaseType(void)>;

    public:
      FillGenerator(Initializer i) : Generator<CType>(),
        m_initializer(std::move(i)) {}

      void generate(Population& pop) {
        using FitnessType = typename Candidate::FitnessType;

        #pragma omp parallel for
        for (size_t i = 0; i < pop.size(); i++) {
          if (!pop[i].alive) {
            pr::progeny(pop[i]) = m_initializer();
            pr::fitness(pop[i]) = FitnessType{};
            pop[i].alive = true;
          }
        }

      }

    private:
      Initializer m_initializer;
  };
}
#endif
