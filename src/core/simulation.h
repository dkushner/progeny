#ifndef SIMULATION_H
#define SIMULATION_H

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
  public:
    using Population = std::vector<Progeny>;

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
    Simulation(Generator<Progeny> g, Evaluator<Progeny> e, Progenator<Progeny> p) :
      m_generator(g), m_evaluator(e), m_progenator(p) {};
    //! Destructor for Simulation.
    virtual ~Simulation() {}

    virtual void evolve(int size, int elites, Breakpoint<Progeny> bp) = 0;
    virtual void evolve(int size, int elites, Population seed, 
        Breakpoint<Progeny> bp) = 0;

    virtual void addObserver(Observer<Progeny> o) = 0;
    virtual void removeObserver(Observer<Progeny> o) = 0;

  private:
    Simulation() {};

  protected:
    Generator<Progeny> m_generator;
    Evaluator<Progeny> m_evaluator;
    Progenator<Progeny> m_progenator;
};

#endif
