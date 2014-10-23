#include "vm.hpp"

#ifndef VM2_EXECUTIONENGINE_HPP
#define VM2_EXECUTIONENGINE_HPP

namespace VM {

  GValue executeFunction(GFunction*, GValue* arguments);
  GValue executeInstructions(GInstruction*, GValue*);
}


#endif
