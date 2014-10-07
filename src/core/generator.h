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
template <typename Progeny>
class Generator {
  public:
    using Population = std::vector<Progeny>;
 
  public:
    Generator() {};
    virtual ~Generator() {};

    virtual Population generate(int size) {}
    virtual Population generate(Population seed, int size) {}
};

#endif
