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

namespace pr {

  template <size_t X>
  struct Cross;

  template <typename CType, class Enable = void>
  class Crossover;

  template <typename CType>
  class Crossover<
    CType,
    typename std::enable_if<
      !is_static_container<typename CType::BaseType>::value &&
      has_value_type<typename CType::BaseType>::value
    >::type
  > : public Mutator<CType> {

    public:
      using Candidate = typename Mutator<CType>::Candidate;
      using Population = typename Mutator<CType>::Population;

    public:
      Crossover(int points) : Mutator<CType>(), m_points(points) {};

      void mutate(Population& pop) {

        typename std::vector<Candidate>::iterator ita = 
          std::partition(pop.begin(), pop.end(), [](const Candidate& can) {
            return !can.alive;
          });

        auto itb = ita + 1;
        for (; itb != pop.end() && ita != pop.end(); ita += 2, itb += 2) {

          BType a = pr::progeny(*ita);
          BType n_a;

          BType b = pr::progeny(*itb);
          BType n_b;

          auto a_params = param_type{1, (int)a.size() - 1};
          auto b_params = param_type{1, (int)b.size() - 1};

          // Generate and sort a list of crossover points for a.
          std::vector<int> a_points(m_points);
          std::generate_n(a_points.begin(), m_points, [&]() {
            return m_distribution(m_generator, a_params);
          });
          std::sort(std::begin(a_points), std::end(a_points));

          // Generate and sort a list of crossover points for b.
          std::vector<int> b_points(m_points);
          std::generate_n(b_points.begin(), m_points, [&]() {
            return m_distribution(m_generator, b_params);
          });
          std::sort(std::begin(b_points), std::end(b_points));

          auto itr_a = a.begin();
          auto itr_b = b.begin();

          for (int i = 0; i < m_points; i++) {
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

          if (m_points % 2) {
            n_a.append(itr_a, a.end());
            n_b.append(itr_b, b.end());
          } else {
            n_a.append(itr_b, b.end());
            n_b.append(itr_a, a.end());
          }

          pr::progeny(*ita) = n_a;
          pr::progeny(*itb) = n_b;
        }
      }

    private:
      using param_type = std::uniform_int_distribution<>::param_type;
      using BType = typename Candidate::BaseType;

    private:
      const int m_points;
      std::default_random_engine m_generator;
      std::uniform_int_distribution<int> m_distribution;
  };

  //! Specialization for statically sized containers.
  template <typename CType> 
  class Crossover<
    CType,
    typename std::enable_if<
      is_static_container<typename CType::BaseType>::value
    >::type
  > : public Mutator<CType> {

    public:
      using Candidate = typename Mutator<CType>::Candidate;
      using Population = typename Mutator<CType>::Population;

    public:
      Crossover(int points) : Mutator<CType>(), m_points(points) {};

      void mutate(Population& pop) {

        std::random_device rd;
        std::default_random_engine gen(rd());
        std::uniform_int_distribution<int> dist{1, Size - 1};
        m_mask.reset();

        // TODO: Unroll on N as it is known at compile time.
        // TODO: Use specified instead of default once wrapper is created.
        auto chunk = Mask{};
        for (int i = 0; i < m_points; i++) {
          chunk.set();
          m_mask ^= (chunk >> dist(gen)); 
        }

        typename std::vector<Candidate>::iterator ita = 
          std::partition(pop.begin(), pop.end(), [](const Candidate& can) {
            return !can.alive;
          });
        auto itb = ita + 1;

        for (; ita != pop.end() && itb != pop.end(); ita += 2, itb += 2) {
          Cross<Size-1>::cross(*ita, *itb, m_mask);
        }
      }

    protected: 
      static const size_t Size = std::tuple_size<typename CType::BaseType>::value;
      using Mask = std::bitset<Size>;

    protected:
      const int m_points;
      Mask m_mask;

  };

  template <size_t X> 
  struct Cross {

    template <typename CType>
    using Mask = std::bitset<std::tuple_size<typename CType::BaseType>::value>;

    template <typename CType>
    static void cross(CType& a, CType& b, Mask<CType>& mask) {
      if (mask[X]) {
        std::swap(std::get<X>(pr::progeny(a)), std::get<X>(pr::progeny(b)));
      }
      Cross<X-1>::cross(a, b, mask);
    }
  };

  template <>
  struct Cross<0> {

    template <typename CType>
    using Mask = std::bitset<std::tuple_size<typename CType::BaseType>::value>;

    template <typename CType>
    static void cross(CType& a, CType& b, Mask<CType>& mask) {
      if (mask[0]) {
        std::swap(std::get<0>(pr::progeny(a)), std::get<0>(pr::progeny(b)));
      }
    }
  };

}
#endif
