#ifndef SELECTOR_H
#define SELECTOR_H


//! Base candidate selection strategy.
template <typename Progeny>
class Selector {

  using Population = std::vector<Progeny>;
  using RankedPopulation = std::vector<std::pair<Progeny, double>>;

  public:
    Selector() {};
    ~Selector() {};

    Population select(RankedPopulation&& rpop, int count, bool natural = true) {}
    Population select(RankedPopulation& rpop, int count, bool natural = true) {}
};

#endif
