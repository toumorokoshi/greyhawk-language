#include "vm.hpp"

#ifndef VM2_EXECUTIONENGINE_HPP
#define VM2_EXECUTIONENGINE_HPP

namespace VM {

  void executeFunction(GFunction*, GObject**, int);
  void executeInstructions(GInstruction*, GObject**);
}


#endif
