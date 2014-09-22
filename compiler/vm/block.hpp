#include <vector>
#include "scope.hpp"
#include "statement.hpp"

#ifndef VM_BLOCK_HPP
#define VM_BLOCK_HPP

namespace VM {

  class VMBlock {
  public:
    std::vector<VMStatement*> statements;

    void execute(VMScope& scope) {
      for (auto statement : statements) {
        statement->execute(scope);
      }
    }

  };
}

#endif
