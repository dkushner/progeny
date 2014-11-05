#ifndef SIMULATION_H
#define SIMULATION_H

#include <thread>
#include <condition_variable>
#include <chrono>
#include <boost/asio.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/shared_mutex.hpp>

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
      return ProtoSimulation<GType, EType, SType, MType>(g, e, s, m);
    }

    typedef struct {
      size_t generation = 0;
      double meanFitness = 0.0;
      double fitnessVariance = 0.0;
      CType bestCandidate;
      double elapsedTime = 0.0;
    } Data;
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
    using Data = typename pr::Simulation<Candidate>::Data;
    using Handler = std::function<void(const Data&)>;

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

      ProtoSimulation(ProtoSimulation&&) = default;
      ProtoSimulation& operator=(ProtoSimulation&&) = default;

      ProtoSimulation(const ProtoSimulation&) = delete;
      ProtoSimulation& operator=(const ProtoSimulation&) = delete;

      //! Destructor for Simulation.
      ~ProtoSimulation() {}

      void evolve(int size, int elites, Breakpoint bp) {

        Data obs_data;
        boost::shared_mutex obs_mutex;
        std::condition_variable_any obs_cv;
        auto start_time = std::chrono::system_clock::now();

        // Spin up simulation.
        volatile bool running = true;

        // Preallocate a population.
        m_population.resize(size);

        // Allocate storage for our observer threads.
        boost::thread_group threads;

        // Build observer threads from function-event pairs.
        for (auto obs : m_observers) {
          
          // Wrap the handler function in a thread bound with a constant
          // reference to our population.
          boost::thread* ob_thread = new boost::thread([&]() {
            while (running) {
              boost::shared_lock<boost::shared_mutex> lock(obs_mutex);
              obs_cv.wait(lock);
              obs(std::cref(obs_data));
            }
          });

          // Add our new thread to the group.
          threads.add_thread(ob_thread);
        }

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
          {
            boost::unique_lock<boost::shared_mutex> uniq(obs_mutex);
            typename Candidate::FitnessType sum_fit{};
            typename Candidate::FitnessType sum_sqrfit{};

            #pragma omp parallel for reduction(+ : sum_fit, sum_sqrfit)
            for (int i = 0; i < m_population.size(); i++) {
              sum_fit = sum_fit + pr::fitness(m_population[i]);
              sum_sqrfit = pr::fitness(m_population[i]) * 
                pr::fitness(m_population[i]);
            }

            auto elapsed = std::chrono::system_clock::now() - start_time;
            elapsed = std::chrono::duration_cast<std::chrono::seconds>(elapsed);

            obs_data.meanFitness = sum_fit / m_population.size();
            obs_data.fitnessVariance = (sum_sqrfit - (sum_fit * sum_fit) / 
                m_population.size()) / (m_population.size() - 1);
            obs_data.elapsedTime = elapsed.count();
            obs_data.generation++;
          }
          obs_cv.notify_all();

        } while (!bp(m_population, elite));

        // Spin down simulation. Join all observer threads.
        running = false;
        obs_cv.notify_all();
        threads.join_all();

        std::cout << "FINISHED" << std::endl;
        std::cout << pr::progeny(elite) << std::endl;
      }

      void evolve(int size, int elites, Population& seed, Breakpoint bp) {}

      size_t addObserver(Handler hand) {
        m_observers.push_back(hand);
        return m_observers.size() - 1;
      }

      void removeObserver(size_t oid) {
        m_observers.erase(m_observers.begin() + oid);
      }

    protected:
      Generator m_generator;
      Evaluator m_evaluator;
      Selector m_selector;
      Mutator m_pipeline;

    private:
      Population m_population;
      std::vector<Handler> m_observers;
  };
}

#endif
