#ifndef FILL_GENERATOR_H
#define FILL_GENERATOR_H

#include <string>
#include <algorithm>

#include "../core/generator.h"

namespace pr {
  template <typename CType, class Enable = void>
  class FillGenerator;

  // WTF?
  template <typename CType>
  class FillGenerator<
    CType,
    typename std::enable_if<
      std::true_type::value
    >::type
  >: public Generator<CType> {

    public:
      using typename Generator<CType>::Candidate;
      using typename Generator<CType>::Population;
      using typename Generator<CType>::Initializer;

    public:
      FillGenerator(Initializer i) : Generator<CType>(i) {}

    public:
      void generate(Population& seed, size_t size) {

        // Ensure that we can contain at _least_ size candidates in the
        // population.
        seed.reserve(size);

        if (seed.size() > size) {
          seed.erase(seed.begin() + size, seed.end());
        } else {
          size_t fill_count = size - seed.size();
          std::generate_n(std::back_inserter(seed), fill_count, this->m_initializer);
        }
      }
  };

}
#endif
