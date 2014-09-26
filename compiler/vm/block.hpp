#include <vector>
#include "scope.hpp"
#include "statement.hpp"

#ifndef VM_BLOCK_HPP
#define VM_BLOCK_HPP

namespace VM {

  class VMBlock {
  public:
    std::vector<VMStatement*> statements;

    VMObject* execute(VMScope& scope) {

      for (auto statement : statements) {
        auto object = statement->execute(scope);
        if (object != NULL) {
          return object;
        }

      }

      return NULL;
    }

  };
}

#endif
