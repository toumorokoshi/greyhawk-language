#include "codegenerator.hpp"
#include "exceptions.hpp"
#include "../lexer/tokens.hpp"
#include <iostream>

using namespace llvm;

namespace codegen {

  Value* CodeGenerator::generate(NClassInstantiation& n) {
    auto type = singleTypeOf(n.type, getContext().classes);
    Value* newInstance = builder.CreateAlloca(type);
    int index = 0;
    for (auto parameter: n.parameters) {
      // first we create the GEP to extract the element pointer
      SmallVector<Value*, 8> GEP;
      GEP.push_back(ConstantInt::get(getGlobalContext(), APInt(32, 0, false)));
      GEP.push_back(ConstantInt::get(getGlobalContext(), APInt(32, index, false)));
      auto pointerToAttribute = builder.CreateInBoundsGEP(newInstance, GEP);

      builder.CreateStore(pointerToAttribute, generate(*parameter));
      index++;
    }
    return newInstance;
  }
}
