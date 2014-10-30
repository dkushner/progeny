#ifndef SIMULATION_H
#define SIMULATION_H

#include <thread>
#include <condition_variable>
#include <mutex>

#include "generator.h"
#include "evaluator.h"
#include "selector.h"
#include "mutator.h"

namespace pr {

  template <typename GType, typename EType, typename SType, typename MType>
  class ProtoSimulation;

  template <typename CType>
  struct Simulation {

    template <
      typename GType, 
      typename EType, 
      typename SType, 
      typename MType
    >
    static typename std::enable_if<
      std::is_base_of<typename pr::Generator<CType>, GType>::value &&
      std::is_base_of<typename pr::Evaluator<CType>, EType>::value &&
      std::is_base_of<typename pr::Selector<CType>, SType>::value &&
      std::is_base_of<typename pr::Mutator<CType>, MType>::value,
      ProtoSimulation<GType, EType, SType, MType> 
    >::type build(GType g, EType e, SType s, MType m) {
      return std::move(ProtoSimulation<GType, EType, SType, MType>(g, e, s, m));
    }
  };

  enum EventFlags {
    GENERATED = 1,
    EVALUATED = 2,
    SELECTED = 4,
    MUTATED = 8
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
    
    using Candidate = typename GType::Candidate;
    using Population = typename GType::Population;
    using Breakpoint = std::function<bool(const Population&, Candidate&)>;
    using Observer = std::function<void(const Population&)>;

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
      ProtoSimulation(Generator g, Evaluator e, Selector s, Mutator p) :
        m_generator(std::forward<Generator>(g)), 
        m_evaluator(std::forward<Evaluator>(e)), 
        m_selector(std::forward<Selector>(s)), 
        m_pipeline(std::forward<Mutator>(p)) {};

      //! Destructor for Simulation.
      ~ProtoSimulation() {}

      void evolve(int size, int elites, Breakpoint bp) {
        m_running = true;

        std::vector<std::thread> threads(m_observers.size());

        auto start = m_observers.begin();
        auto finish = m_observers.end();
        std::transform(start, finish, threads.begin(), [&](Observer& obs) {
          return std::thread([&](const Population& pop) {
            while (m_running) {
              obs(pop);
            }
          }, std::cref(m_population));
        });

        m_population.reserve(size);

        m_generator.generate(m_population, size);
        emit(EventFlags::GENERATED);

        m_evaluator.evaluate(m_population);
        emit(EventFlags::EVALUATED);

        Candidate elite;
        while (!bp(m_population, elite)) {
          m_selector.select(m_population, elites, false);
          emit(EventFlags::SELECTED);

          m_pipeline.mutate(m_population);
          emit(EventFlags::MUTATED);

          m_generator.generate(m_population, size);
          emit(EventFlags::GENERATED);

          m_evaluator.evaluate(m_population);
          emit(EventFlags::EVALUATED);
        }

        m_running = false;
        std::for_each(threads.begin(), threads.end(), [](std::thread& t) {
          t.join();
        });

        std::cout << pr::progeny(elite) << std::endl;
      }

      void evolve(int size, int elites, Population& seed, Breakpoint bp) {}

      void registerObserver(EventFlags s, Observer obs) {
        m_observers.push_back(obs);
      }

    protected:
      Generator m_generator;
      Evaluator m_evaluator;
      Selector m_selector;
      Mutator m_pipeline;

    private:
      Population m_population;
      EventFlags m_state;
      volatile bool m_running = false;
      std::vector<Observer> m_observers;

    private:
      void emit(EventFlags s) {
        m_state = s;
      }
  };

}

#endif
