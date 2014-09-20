#ifndef SELECTOR_H
#define SELECTOR_H


//! Base candidate selection strategy.
template <typename Progeny, typename Impl>
class Selector {

  using Population = std::vector<Progeny>;
  using RankedPopulation = std::vector<std::pair<Progeny, double>>;

  public:
    Selector(const Selector&) = delete;
    Selector& operator=(const Selector&) = delete;

    Selector() = default;
    Selector(Selector&&) = default;
    Selector& operator=(Selector&&) = default;
    virtual ~Selector() {};

    Population select(RankedPopulation&& rpop) {
      return static_cast<Impl*>(this)->select(std::forward<RankedPopulation>(rpop));
    }
};

#endif
