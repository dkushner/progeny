#ifndef GENERATOR_H
#define GENERATOR_H

#include <functional>
#include <vector>
#include <random>

//! Abstract base class of population generators.
/*!
 *  This abstract base class may be extended to provide specialized 
 *  population members for any given population type.
 */
template <typename Progeny, typename Impl>
class Generator {
  public:
    using Population = std::vector<Progeny>;
 
  public:
    Generator(std::function<int()> rng) : m_rng(rng) {};
    virtual ~Generator() {};

    Population generate(int size) {
      return static_cast<Impl*>(this)->generate(std::forward<int>(size));
    }
    Population generate(Population seed, int size) {
      return static_cast<Impl*>(this)->generate(
          std::forward<Population>(seed),
          std::forward<int>(size));
    }
  
  private:
    Generator() {};

  protected:
    std::function<int()> m_rng;
};

#endif
