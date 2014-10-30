#ifndef SIMULATION_H
#define SIMULATION_H

#include "generator.h"
#include "evaluator.h"
#include "selector.h"
#include "mutator.h"

namespace pr {

  template <typename CType>
  struct Simulation {

    template <
      typename GType, 
      typename EType, 
      typename SType, 
      typename MType, 
      class Enable = void
    >
    static auto build(GType g, EType e, SType s, MType m)  ->
      ProtoSimulation<GType, EType, SType, MType> {}

    template <
      typename GType, 
      typename EType, 
      typename SType, 
      typename MType
    >
    static ProtoSimulation<GType, EType, SType, MType> build<
      GType,
      Etype,
      SType,
      MType,
      typename std::enable_if<
        std::is_base_of<typename pr::Generator<CType>, GType>::value &&
        std::is_base_of<typename pr::Evaluator<CType>, EType>::value &&
        std::is_base_of<typename pr::Selector<CType>, SType>::value &&
        std::is_base_of<typename pr::Mutator<CType>, MType>::value
      >::type
    >(GType g, EType e, SType s, MType m) {
      return std::move(ProtoSimulation<GType, EType, SType, MType>(g, e, s, m));
    }
  };

  //! Encapsulated instance of an evolutionary system.
  /*!
  *  The simulation is the core executor of an evolutionary process. It 
  *  brings together the many independent components to iteratively generate,
  *  evaluate and mutate a population of the given type.
  *  \tparam Progeny The type of population member that will be evolved.
  */
  template <typename GType, typename EType, typename SType, typename MType>
  class ProtoSimulation {

    using Generator = GType;
    using Evaluator = EType;
    using Selector = SType;
    using Mutator = MType;
    
    using Population = 
    using Breakpoint = std::function<bool(P)>;

    public:
      //! Constructor for Simulation.
      /*!
      *  \param g The generator that will provide the population.
      *  \param e The evaluator that will assess the fitness of every
      *  member in the population.
      *  \param p The progenator that will mutate and recombine the evaluated
      *  population.
      *  \sa Generator, Evaluator, Progenator
      */
      Simulation(Generator g, Evaluator e, Selector s, Mutator p) :
        m_generator(std::forward<Generator>(g)), 
        m_evaluator(std::forward<Evaluator>(e)), 
        m_selector(std::forward<Selector>(s)), 
        m_pipeline(std::forward<Mutator>(p)) {};

      //! Destructor for Simulation.
      ~Simulation() {}

      void evolve(int size, int elites, Breakpoint bp) {

        Population pop = m_generator.generate(size);
        RankedPopulation ranked = m_evaluator.evaluate(pop);

        Progeny elite;
        while (!bp(ranked, elite)) {
          auto selected = m_selector.select(ranked, elites, false);
          pop = m_generator.generate(m_pipeline(selected), size);
          ranked = m_evaluator.evaluate(pop);
        }

        std::cout << elite << std::endl;
        return;
      }

      void evolve(int size, int elites, Population& seed, Breakpoint bp) {}

    protected:
      Generator m_generator;
      Evaluator m_evaluator;
      Selector m_selector;
      Mutator m_pipeline;
  };

}

#endif
