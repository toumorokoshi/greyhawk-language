#include "codegen.hpp"

using namespace parser;
using namespace VM;

namespace codegen {

  VM::VMBlock* generateBlock(PBlock* pblock, CScope* scope) {
    auto block = new VMBlock();
    for (auto pstatement : pblock->statements) {
      block->statements.push_back(generateStatement(pstatement, scope));
    }
    return block;
  }

  VM::VMStatement* generateStatement(PStatement* pstatement, CScope* scope) {
    if (auto
  }
}
