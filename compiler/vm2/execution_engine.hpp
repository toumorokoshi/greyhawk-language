#include "vm.hpp"

#ifndef VM2_EXECUTIONENGINE_HPP
#define VM2_EXECUTIONENGINE_HPP

namespace VM {

  void executeInstruction(GInstruction&);
  void executeFunction(GFunction*);
}


#endif
