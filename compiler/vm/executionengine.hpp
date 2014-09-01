#include "../parser/node.hpp"
#include "./scope.hpp"

#ifndef VM_EXECUTIONENGINE_HPP
#define VM_EXECUTIONENGINE_HPP

namespace VM {

  class VMExecutionEngine {
  public:
    VMScope scope;
    void executeBlock(NBlock&);
    void executeStatement(NStatement*);
    void executeExpression(NExpression*);
  };
}

#endif
