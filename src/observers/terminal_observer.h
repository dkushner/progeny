#ifndef TERMINAL_OBSERVER_H
#define TERMINAL_OBSERVER_H

#include <iostream>

#include "../core/observer.h"

namespace pr {
  template <typename CType>
  class TerminalObserver : public Observer<CType> {

    using ProgressData = typename pr::Observer<CType>::ProgressData;

    protected:
      void onProgress(const ProgressData& data) {
        std::cout << data.elapsedTime << '\xd';
      }
  };
}

#endif
