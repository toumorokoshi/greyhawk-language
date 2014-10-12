#include "codegenerator.hpp"
#include "exceptions.hpp"
#include "../lexer/tokens.hpp"
#include <iostream>

using namespace llvm;

namespace codegen {

  Type* singleTypeOf(NSingleType& type, ClassMap& classes)
  {
    if (type.name.compare("Int") == 0) {
      return Type::getInt64Ty(getGlobalContext());

    } else if (type.name.compare("Double") == 0) {
      return Type::getDoubleTy(getGlobalContext());

    } else if (type.name.compare("Bool") == 0) {
      return Type::getInt1Ty(getGlobalContext());

    } else if (type.name.compare("Void") == 0) {
      return Type::getVoidTy(getGlobalContext());

    } else {
      auto classIterator = classes.find(type.name);
      if (classIterator != classes.end()) {
        return classIterator->second->type;
      }
    }
    throw CodeGenException("Unable to detect type of " + type.name);
  }

  /* Returns an LLVM type based on the identifier */
  Type* typeOf(NType* type, ClassMap& classes) {

    if (NSingleType* n = dynamic_cast<NSingleType*>(type)) {
      return singleTypeOf(*n, classes);

    } else if (NArrayType* n = dynamic_cast<NArrayType*> (type)) {
      auto element_type = singleTypeOf(n->type, classes);
      return ArrayType::get(element_type, 0);

    } else {
      throw CodeGenException("Expected Type!");

    }

  }

}
