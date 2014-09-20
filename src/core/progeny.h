#ifndef PROGENY_H
#define PROGENY_H

#define PROGENY_TYPE(...)                     \
    using Progeny = __VA_ARGS__;              \
    using Population = std::vector<Progeny>;  \
    using RankedPopulation = std::vector<     \
      std::pair<Progeny, double>              \
    >

#include <tuple>
#include <map>
#include <vector>

/*
template <
  unsigned int N, 
  unsigned int X,
  class Enable = void
>
struct ProgenyPrinter {
  template <typename... Ps>
  static void print(std::ostream& os, const Progeny<Ps...>& p) {
    ProgenyPrinter<N - 1, X>::print(os, p);
    os << std::get<N>(p) << ", ";
  }
};

template <
  unsigned int N,
  unsigned int X
>
struct ProgenyPrinter<N, X, typename std::enable_if<N == X>::type> {
  template <typename... Ps>
  static void print(std::ostream& os, const Progeny<Ps...>& p) {
    ProgenyPrinter<N - 1, X>::print(os, p);
    os << std::get<N>(p);
  }
};

template <unsigned int X>
struct ProgenyPrinter<0, X> {
  template <typename... Ps>
  static void print(std::ostream& os, const Progeny<Ps...>& p) {
    os << std::get<0>(p) << ", ";
  }
};

template <>
struct ProgenyPrinter<0, 0> {
  template <typename... Ps>
  static void print(std::ostream& os, const Progeny<Ps...>& p) {
    os << std::get<0>(p);
  }
};

template <typename... Ps> 
std::ostream& operator<<(std::ostream& os, const Progeny<Ps...>& p) {
  os << "(";
  ProgenyPrinter<sizeof...(Ps) - 1, sizeof...(Ps) - 1>::print(os, p);   
  os << ")";
  return os;
}

namespace boost {
  namespace serialization {
    template <unsigned int N>
    struct ProgenySerializer {
      template <class Archive, typename... Ps>
      static void serialize(Archive& a, Progeny<Ps...>& p, const unsigned int v) {
        a & std::get<N - 1>(p); 
        ProgenySerializer<N - 1>::serialize(a, p, v);
      }
    };

    template <>
    struct ProgenySerializer<0> {
      template <class Archive, typename... Ps>
      static void serialize(Archive& a, Progeny<Ps...>& p, const unsigned int v) {}
    };

    template <class Archive, typename... Ps>
    void serialize(Archive& a, Progeny<Ps...>& p, const unsigned int v) {
      ProgenySerializer<sizeof...(Ps)>::serialize(a, p, v);
    }
  }
}

*/


#endif
