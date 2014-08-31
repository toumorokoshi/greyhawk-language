#include "../parser/node.hpp"

#ifndef VM_EXECUTIONENGINE_HPP
#define VM_EXECUTIONENGINE_HPP

namespace VM {

  class VMExecutionEngine {
  public:
    void executeBlock(NBlock);
  };
}

#endif
