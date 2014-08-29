#include "codegenerator.hpp"
#include "exceptions.hpp"
#include "../lexer/tokens.hpp"
#include <iostream>

using namespace llvm;

namespace codegen {

  Value* CodeGenerator::generate(NClassInstantiation& n) {
    auto type = singleTypeOf(n.type, getContext().classes);
  }
}
