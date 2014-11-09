#ifndef OBSERVER_H
#define OBSERVER_H

#include <boost/signals2.hpp>

#include "simulation.h"

namespace pr {

  template <typename CType>
  class Simulation;

  template <typename CType>
  class Observer {
    
    public: 
      using ProgressData = typename pr::Simulation<CType>::ProgressData;

    public:
      Observer() = default;
      virtual ~Observer() = default;

      void bind(Simulation<CType>& sim) {
        m_progconn = 
          sim.m_progress.connect(boost::bind(&Observer::onProgress, this, _1));
        m_modconn = 
          sim.m_modified.connect(boost::bind(&Observer::onModified, this, _1));
      }

    protected: 
      virtual void onProgress(const ProgressData&) {};
      virtual void onModified(const CType&) {};

    private:
      boost::signals2::scoped_connection m_progconn;
      boost::signals2::scoped_connection m_modconn;

  };
}

#endif
