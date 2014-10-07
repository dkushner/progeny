#ifndef UTILITY_H
#define UTILITY_H

template <
  unsigned int N, 
  unsigned int X,
  class Enable = void
>
struct TuplePrinter {
  template <typename... Ps>
  static void print(std::ostream& os, const std::tuple<Ps...>& p) {
    TuplePrinter<N - 1, X>::print(os, p);
    os << std::get<N>(p) << ", ";
  }
};

template <
  unsigned int N,
  unsigned int X
>
struct TuplePrinter<N, X, typename std::enable_if<N == X>::type> {
  template <typename... Ps>
  static void print(std::ostream& os, const std::tuple<Ps...>& p) {
    TuplePrinter<N - 1, X>::print(os, p);
    os << std::get<N>(p);
  }
};

template <unsigned int X>
struct TuplePrinter<0, X> {
  template <typename... Ps>
  static void print(std::ostream& os, const std::tuple<Ps...>& p) {
    os << std::get<0>(p) << ", ";
  }
};

template <>
struct TuplePrinter<0, 0> {
  template <typename... Ps>
  static void print(std::ostream& os, const std::tuple<Ps...>& p) {
    os << std::get<0>(p);
  }
};

template <typename... Ps> 
std::ostream& operator<<(std::ostream& os, const std::tuple<Ps...>& p) {
  os << "(";
  TuplePrinter<sizeof...(Ps) - 1, sizeof...(Ps) - 1>::print(os, p);
  os << ")";
  return os;
}

template <size_t N, typename T>
std::ostream& operator<<(std::ostream& o, const std::array<T, N>& arr) {
  std::copy(arr.cbegin(), arr.cend(), std::ostream_iterator<T>(o, " "));
  return o;
}

namespace boost {
  namespace serialization {
    template <unsigned int N>
    struct TupleSerializer {
      template <class Archive, typename... Ps>
      static void serialize(Archive& a, std::tuple<Ps...>& p, const unsigned int v) {
        a & std::get<N - 1>(p); 
        TupleSerializer<N - 1>::serialize(a, p, v);
      }
    };

    template <>
    struct TupleSerializer<0> {
      template <class Archive, typename... Ps>
      static void serialize(Archive& a, std::tuple<Ps...>& p, const unsigned int v) {}
    };

    template <class Archive, typename... Ps>
    void serialize(Archive& a, std::tuple<Ps...>& p, const unsigned int v) {
      TupleSerializer<sizeof...(Ps)>::serialize(a, p, v);
    }
  }
}

#endif
