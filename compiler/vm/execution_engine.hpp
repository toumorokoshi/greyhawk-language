#include "function.hpp"
#include "types/primitives.hpp"
#include <map>

#ifndef VM2_EXECUTIONENGINE_HPP
#define VM2_EXECUTIONENGINE_HPP

namespace VM {

  GValue executeInstructions(GModules*, GInstruction*, GEnvironmentInstance&);
}


#endif
