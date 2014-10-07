#ifndef STRING_GENERATOR_H
#define STRING_GENERATOR_H

#include <string>
#include <algorithm>
#include "../core/generator.h"

class StringGenerator : public Generator<std::string> {

  const std::string DEFAULT_VALID = "abcdefghijklmnopqrstuvwxyz";

  using Population = std::vector<std::string>;

  public:
    StringGenerator(int length) : 
      m_length(length), m_valid(DEFAULT_VALID) {};
    StringGenerator(int length, std::string valid) : 
      m_length(length), m_valid(valid) {};

    Population generate(int size) {
      std::default_random_engine gen;
      std::uniform_int_distribution<int> dist(0, m_valid.size() - 1);

      Population p(size);
      std::generate(p.begin(), p.end(), [&] {
        std::string s(m_length, 0);
        std::generate(s.begin(), s.end(), [&] {
          return m_valid[dist(gen)];
        });
        return s;
      });
      return p;
    }

    Population generate(Population seed, int size) {
      std::random_shuffle(seed.begin(), seed.end());

      
      if (seed.size() >= size) {
        return Population(seed.begin(), seed.begin() + size);
      } else {
        Population p(size - seed.size());
        for (auto& m : p) {
          std::string s(m_length, '\0');
          std::default_random_engine gen;
          std::uniform_int_distribution<int> dist(0, m_valid.size() - 1);

          std::generate(s.begin(), s.end(), [&] {
            return m_valid[dist(gen)];
          });
          m = s;
        }
        seed.insert(seed.end(), p.begin(), p.end());
        return seed;
      }
    }

  protected:
    int m_length;
    std::string m_valid;
};

#endif
