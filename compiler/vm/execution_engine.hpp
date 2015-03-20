#include "function.hpp"
#include <map>

#ifndef VM2_EXECUTIONENGINE_HPP
#define VM2_EXECUTIONENGINE_HPP

namespace VM {

  GObject executeFunction(GModules*, GFunction*, GValue* arguments);
  GObject executeInstructions(GModules*, GInstruction*, GScopeInstance&);
}


#endif
