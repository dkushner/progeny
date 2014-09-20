#ifndef STRING_GENERATOR_H
#define STRING_GENERATOR_H

#include <string>
#include <algorithm>
#include "../core/generator.h"

class StringGenerator : Generator<std::string, StringGenerator> {

  const std::string DEFAULT_VALID = "abcdefghijklmnopqrstuvwxyz";

  using Population = std::vector<std::string>;

  public:
    StringGenerator(int length, std::function<int()> rng) : Generator(rng),
      m_length(length), m_valid(DEFAULT_VALID) {};
    StringGenerator(int length, std::string valid, std::function<int()> rng) : 
      Generator(rng), m_length(length), m_valid(valid) {};

    Population generate(int size) {
      Population p(size);
      for (auto& m : p) {
        std::string s(m_length, '\0');
        std::generate(s.begin(), s.end(), [&] {
          return m_valid[m_rng() % m_valid.size()];
        });
        m = s;
      }
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
          std::generate(s.begin(), s.end(), [&] {
            return m_valid[m_rng() % m_valid.size()];
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
