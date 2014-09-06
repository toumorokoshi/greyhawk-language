#include <vector>
#include "scope.hpp"
#include "statement.hpp"

#ifndef VM_BLOCK_HPP
#define VM_BLOCK_HPP

namespace VM {

  class VMBlock {
  public:
    VMScope scope;
    std::vector<VMStatement*> statements;

    VMBlock(VMScope* parentScope) :
      scope(VMScope(parentScope)) {}

    VMObject* execute() {
      for (auto statement : statements) {
        statement->execute(scope);
      }
      return NULL;
    }

  };
}

#endif
