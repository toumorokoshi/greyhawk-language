#include "vm.hpp"
#include <map>

#ifndef VM2_EXECUTIONENGINE_HPP
#define VM2_EXECUTIONENGINE_HPP

namespace VM {

  GValue executeFunction(GVM*, GFunction*, GValue* arguments);
  GValue executeInstructions(GVM*, GInstruction*, GValue*);
}


#endif
