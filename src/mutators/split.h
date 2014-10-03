#ifndef SPLIT_H
#define SPLIT_H

#include "../core/mutator.h"
#include "../core/type_traits.h"

template <unsigned int X>
struct Split;

template <typename Progeny, class Enable = void>
class Split;

template <typename Progeny>
class Split<
  Progeny, 
  typename std::enable_if<is_static_container<Progeny>::value>::type
> : public Mutator<Progeny, Split<Progeny>> {

  using Population = std::vector<Progeny>;

  public:
    static const size_t Arity;

  public:
    Split(Mutator<Progeny>... mutators) : Mutator<Progeny, Split<N, Progeny>>(), 
      Arity(sizeof...(mutators)) {};

    Population mutate(Population&& pop) {
    }
};

#endif