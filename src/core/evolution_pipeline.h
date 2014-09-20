#ifndef EVOLUTION_PIPELINE_H
#define EVOLUTION_PIPELINE_H

#include "mutator.h"

//! Encapsulated series of evolutionary operators.
/*!
 *  This class encapsulates a series of evolutionary operators 
 *  and provides syntax sugar for easily and fluently constructing 
 *  evolutionary pipelines from constituent operations.
 */
template <typename Inner, typename Outer>
class EvolutionPipeline {
  public:
    EvolutionPipeline() = delete;
    EvolutionPipeline(const EvolutionPipeline&) = delete;
    EvolutionPipeline& operator=(const EvolutionPipeline&) = delete;

    EvolutionPipeline(EvolutionPipeline&&) = default;
    EvolutionPipeline& operator=(EvolutionPipeline&&) = default;

    EvolutionPipeline(Inner i, Outer o) :
      m_inner(std::forward<Inner>(i)), m_outer(std::forward<Outer>(i)) {};

    /*!
     *  \tparam O Type of the evolutionary operator to be appended to the
     *  pipeline. This is deduced at compile-time automatically.
     *  \param otr The evolutionary operator instance to append to the 
     *  pipeline.
     *  \returns The concatenated evolutionary pipeline.
     */
    template <typename O>
    auto operator>>=(O&& otr) const -> EvolutionPipeline<
      EvolutionPipeline<Inner, Outer>, decltype(std::forward<O>(otr))
    > {
      return EvolutionPipeline<
        EvolutionPipeline<Inner, Outer>, decltype(std::forward<O>(otr))
      >(*this, std::forward<O>(otr));
    }

    auto inner() -> typename std::add_lvalue_reference<Inner>::type {
      return m_inner;
    }

    auto inner() const -> typename std::add_lvalue_reference<
      typename std::add_const<Inner>::type
    >::type {
      return m_inner;
    }

    auto outer() -> typename std::add_lvalue_reference<Outer>::type {
      return m_outer;
    }

    auto outer() const -> typename std::add_lvalue_reference<
      typename std::add_const<Outer>::type
    >::type {
      return m_outer;
    }

    /*!
     *  Processes the given population using this evolutionary pipeline.
     *
     *  \tparam PopType The population type of concern. This is deduced
     *  at compile-time automatically.
     *  \param p The population to process.
     *  \returns The processed population.
     */
    template <typename PopType>
    auto operator()(PopType p) const -> decltype(Outer()(Inner()(p))) {
      return outer()(inner()(p)); 
    }

  private:
    Inner m_inner; //! The preceding evolutionary pipeline.
    Outer m_outer; //! The appended evolutionary pipeline.
};

#endif
