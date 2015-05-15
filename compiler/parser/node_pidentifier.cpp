#include "nodes.hpp"

using namespace VM;
using namespace codegen;

namespace parser {

  GType* PIdentifier::getType(codegen::GScope* scope) {
    auto object = scope->getObject(name);
    return object != NULL ? object->type : VM::getNoneType();
  }
}
