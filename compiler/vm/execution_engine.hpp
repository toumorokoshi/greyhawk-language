#include "function.hpp"
#include <map>

#ifndef VM2_EXECUTIONENGINE_HPP
#define VM2_EXECUTIONENGINE_HPP

namespace VM {

  GValue executeFunction(GModules*, GFunction*,
                         GEnvironmentInstance& parent,
                         GValue* arguments);
  GValue executeInstructions(GModules*, GInstruction*, GEnvironmentInstance&);
}


#endif
