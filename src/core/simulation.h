#ifndef SIMULATION_H
#define SIMULATION_H

#include <thread>
#include <condition_variable>
#include <boost/signals2.hpp>
#include <chrono>

#include "generator.h"
#include "evaluator.h"
#include "observer.h"
#include "selector.h"
#include "mutator.h"

namespace pr {

  template <
    typename GType, 
    typename EType, 
    typename SType, 
    typename MType, 
    typename CType
  >
  class ProtoSimulation;

  template <typename CType>
  class Simulation {
    friend class Observer<CType>;

    // Public Typedefs
    public:
      typedef struct {
        size_t generation = 0;
        double meanFitness = 0.0;
        double fitnessVariance = 0.0;
        CType bestCandidate;
        double elapsedTime = 0.0;
      } ProgressData;

    public:
      Simulation(Simulation&& sim) {
        boost::swap(m_progress, sim.m_progress);
        boost::swap(m_modified, sim.m_modified);
      }
      Simulation& operator=(Simulation&&) = default;

    // Protected Signal Members
    protected:
      boost::signals2::signal<void(const ProgressData&)> m_progress;
      boost::signals2::signal<void(const CType&)> m_modified;

    protected:
      Simulation() = default;

    public:
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
        ProtoSimulation<GType, EType, SType, MType, CType>
      >::type build(GType g, EType e, SType s, MType m) {
        return ProtoSimulation<GType, EType, SType, MType, CType>(g, e, s, m);
      }

  };

  
  //! Encapsulated instance of an evolutionary system.
  /*!
  *  The simulation is the core executor of an evolutionary process. It 
  *  brings together the many independent components to iteratively generate,
  *  evaluate and mutate a population of the given type.
  *  \tparam Progeny The type of population member that will be evolved.
  */
  template <
    typename GType, 
    typename EType, 
    typename SType, 
    typename MType, 
    typename CType
  >
  class ProtoSimulation : public Simulation<CType> {
    
    using Generator = GType;
    using Evaluator = EType;
    using Selector = SType;
    using Mutator = MType;
    using Candidate = CType;

    using Population = typename pr::Population<CType>;
    using Breakpoint = std::function<bool(const Population&, Candidate&)>;
    using ProgressData = typename pr::Simulation<Candidate>::ProgressData;

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
        m_generator(std::move(g)), m_evaluator(std::move(e)), 
        m_selector(std::move(s)), m_pipeline(std::move(p)) {}

      ProtoSimulation(ProtoSimulation&& otr) = default;
      ProtoSimulation& operator=(ProtoSimulation&&) = default;

      ProtoSimulation(const ProtoSimulation&) = delete;
      ProtoSimulation& operator=(const ProtoSimulation&) = delete;

      //! Destructor for Simulation.
      ~ProtoSimulation() {}

      Candidate evolve(int size, int elites, Breakpoint bp) {

        ProgressData obs_data;
        auto start_time = std::chrono::high_resolution_clock::now();

        // Preallocate a population.
        m_population.resize(size);

        m_generator.generate(m_population);
        m_evaluator.evaluate(m_population);

        Candidate elite;
        do {

          // Select fittest candidates.
          m_selector.select(m_population, elites, false);

          // Mutate fittest candidates.
          m_pipeline.mutate(m_population);

          // Augment population to specified size. Note that this may or may
          // not include the fittest candidates from the previous step as the
          // behavior is determined by the generator.
          m_generator.generate(m_population);

          // Evaluate the new population.
          m_evaluator.evaluate(m_population);

          // Update population statistics.
          typename Candidate::FitnessType sum_fit{};
          typename Candidate::FitnessType sum_sqrfit{};

          #pragma omp parallel for reduction(+ : sum_fit, sum_sqrfit)
          for (int i = 0; i < m_population.size(); i++) {
            sum_fit = sum_fit + pr::fitness(m_population[i]);
            sum_sqrfit = pr::fitness(m_population[i]) * 
              pr::fitness(m_population[i]);
          }

          auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::high_resolution_clock::now() - start_time
          ).count();

          obs_data.meanFitness = sum_fit / m_population.size();
          obs_data.fitnessVariance = (sum_sqrfit - (sum_fit * sum_fit) / 
              m_population.size()) / (m_population.size() - 1);
          obs_data.elapsedTime = elapsed;
          obs_data.generation++;
          this->m_progress(obs_data);

        } while (!bp(m_population, elite));
        return elite;
      }

      Candidate evolve(int size, int elites, Population& seed, Breakpoint bp) {
        m_population = seed;
        return std::move(evolve(size, elites, bp));
      }

    private:
      Generator m_generator;
      Evaluator m_evaluator;
      Selector m_selector;
      Mutator m_pipeline;
      Population m_population;
  };
}

#endif
