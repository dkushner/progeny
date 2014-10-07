#ifndef SIMULATION_H
#define SIMULATION_H

#include "generator.h"
#include "evaluator.h"
#include "selector.h"
#include "mutator.h"


//! Encapsulated instance of an evolutionary system.
/*!
 *  The simulation is the core executor of an evolutionary process. It 
 *  brings together the many independent components to iteratively generate,
 *  evaluate and mutate a population of the given type.
 *  \tparam Progeny The type of population member that will be evolved.
 *  
 */
template <typename Progeny>
class Simulation {

  using Pr = Progeny;
  using Population = std::vector<Progeny>;
  using RankedPopulation = std::vector<std::pair<Progeny, double>>;
  using Breakpoint = std::function<bool(RankedPopulation&, Progeny&)>;

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
    Simulation(Generator<Pr> g, Evaluator<Pr> e, Selector<Pr> s, Mutator<Pr> p) :
      m_generator(std::forward<Generator<Pr>>(g)), 
      m_evaluator(std::forward<Evaluator<Pr>>(e)), 
      m_selector(std::forward<Selector<Pr>>(s)), 
      m_pipeline(std::forward<Mutator<Pr>>(p)) {};

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

    /*
    void addObserver(Observer<Progeny> o) {}; 
    void removeObserver(Observer<Progeny> o) {};
    */

  private:
    Simulation() {};

  protected:
    Generator<Progeny> m_generator;
    Evaluator<Progeny> m_evaluator;
    Selector<Progeny> m_selector;
    Mutator<Progeny> m_pipeline;
};

#endif
