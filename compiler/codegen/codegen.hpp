#include "../parser/nodes.hpp"
#include "../vm/vm.hpp"

#ifndef CODEGEN_CODEGEN_HPP
#define CODEGEN_CODEGEN_HPP

namespace codegen {

  class CScope {
  };

  VM::VMBlock* generateBlock(parser::PBlock*, CScope*);
  VM::VMStatement* generateStatement(parser::PStatement*, CScope*);

}

#endif
