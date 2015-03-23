#include "function.hpp"
#include <map>

#ifndef VM2_EXECUTIONENGINE_HPP
#define VM2_EXECUTIONENGINE_HPP

namespace VM {

  GValue executeFunction(GModules*, GFunction*, GValue* arguments);
  GValue executeInstructions(GModules*, GInstruction*, GScopeInstance&);
}


#endif
