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

template <unsigned int N>
class Crossover<
  N, 
  std::string
> : public Mutator<std::string> {

  using Population = std::vector<std::string>;
  
  public:
    Crossover() : Mutator<std::string>() {};

    Population& operator()(Population&& pop) {
      std::random_shuffle(pop.begin(), pop.end());
      
      for (int x = 0; x < pop.size() - 1; x += 2) {
        std::string a = pop[x];
        std::string n_a;

        std::string b = pop[x + 1];
        std::string n_b;

        using param_type = std::uniform_int_distribution<>::param_type;

        auto a_params = param_type{0, a.size() - 1};
        auto b_params = param_type{0, b.size() - 1};

        // Generate and sort a list of crossover points for a.
        int a_points[N];
        std::generate_n(a_points, N, [&]() {
          return m_distribution(m_generator, a_params);
        });
        std::sort(std::begin(a_points), std::end(a_points));

        // Generate and sort a list of crossover points for b.
        int b_points[N];
        std::generate_n(b_points, N, [&]() {
          return m_distribution(m_generator, b_params);
        });
        std::sort(std::begin(b_points), std::end(b_points));

        auto itr_a = a.begin();
        auto itr_b = b.begin();

        for (int i = 0; i < N; i++) {
          if (i % 2) {
            n_a.append(itr_a, a.begin() + a_points[i]);
            n_b.append(itr_b, b.begin() + b_points[i]);

          } else {
            n_a.append(itr_b, b.begin() + b_points[i]);
            n_b.append(itr_a, a.begin() + a_points[i]);
          }

          itr_a = a.begin() + a_points[i];
          itr_b = b.begin() + b_points[i];
        }

        if (N % 2) {
          n_a.append(itr_a, a.end());
          n_b.append(itr_b, b.end());
        } else {
          n_a.append(itr_b, b.end());
          n_b.append(itr_a, a.end());
        }

        pop[x] = n_a;
        pop[x + 1] = n_b;
      }
    }

    Population& operator()(Population& pop) {
      return operator()(std::move(pop));
    }
  
  private:
    std::default_random_engine m_generator;
    std::uniform_int_distribution<int> m_distribution;
};

//! Specialization for statically sized containers.
template <unsigned int N, typename Progeny> 
class Crossover<
  N, 
  Progeny,
  typename std::enable_if<is_static_container<Progeny>::value>::type
> : public Mutator<Progeny> {

  static const size_t Size = std::tuple_size<Progeny>::value;

  using Population = std::vector<Progeny>;
  using Mask = std::bitset<Size>;

  public:
    Crossover() : Mutator<Progeny>() {};

    Population& operator()(Population&& pop) {
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

      for (int x = 0; x < pop.size() - 1; x += 2) {
        Cross<Size-1>::cross(pop[x], pop[x+1], m_mask);
      }

      return pop;
    }

    Population& operator()(Population& pop) {
      return operator()(std::move(pop));
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
