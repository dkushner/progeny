#ifndef CROSSOVER_H
#define CROSSOVER_H

#include <vector>
#include <functional>
#include <iostream>
#include <utility>
#include <omp.h>
#include <algorithm>
#include <random>
#include <bitset>

#include "../core/mutator.h"
#include "../core/type_traits.h"

template <unsigned int X>
struct Cross;


template <unsigned int N, typename Progeny, class Enable = void>
class Crossover;

template <unsigned int N, typename Progeny>
class Crossover<
  N, 
  Progeny,
  typename std::enable_if<is_static_container<Progeny>::value>::type
> : public Mutator<Progeny, Crossover<N, Progeny>> { 

  static const size_t Size = std::tuple_size<Progeny>::value;

  using Population = std::vector<Progeny>;
  using Mask = std::bitset<Size>;

  public:
    Crossover() : Mutator<Progeny, Crossover<N, Progeny>>() {};

    Population& mutate(Population& pop) {
      // Shuffle our population so that pairings are random.
      std::random_shuffle(pop.begin(), pop.end());

      // TODO: Unroll on N as it is known at compile time.
      // TODO: Use specified instead of default once wrapper is created.
      std::default_random_engine gen;
      std::uniform_int_distribution<int> dist(1, Size - 1);

      auto chunk = Mask{}.set();
      for (int i = 0; i < N; i++) {
        m_mask ^= (chunk >> dist(gen)); 
        chunk.set();
      }

      #pragma omp parallel for 
      for (int x = 0; x < pop.size() - 1; x += 2) {
        Cross<Size - 1>::cross(pop[x], pop[x+1], m_mask);
      }

      return pop;
    }

  private:
    Mask m_mask;

};

template <unsigned int X>
struct Cross {

  template <typename Progeny>
  using Mask = std::bitset<std::tuple_size<Progeny>::value>;

  template <typename Progeny>
  static void cross(Progeny& pA, Progeny& pB, Mask<Progeny>& mask) {
    if (mask[X]) {
      std::swap(std::get<X>(pA), std::get<X>(pB));
    }
    Cross<X-1>::cross(pA, pB, mask);
  }
};

template <>
struct Cross<0> {

  template <typename Progeny>
  using Mask = std::bitset<std::tuple_size<Progeny>::value>;

  template <typename Progeny>
  static void cross(Progeny& pA, Progeny& pB, Mask<Progeny>& mask) {
    if (mask[0]) {
      std::swap(std::get<0>(pA), std::get<0>(pB));
    }
  }
};

#endif
