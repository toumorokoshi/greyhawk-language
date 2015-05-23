#include "nodes.hpp"
#include "exceptions.hpp"

using codegen::GScope;
using VM::GInstruction;
using VM::GOPCODE;
using VM::GOPCODE::ARRAY_LOAD_VALUE;
using VM::GOPCODE::LOAD_CONSTANT_INT;
using VM::GOPCODE::SET;
using VM::GOPARG;
using VM::isTupleType;
using VM::getInt32Type;

namespace parser {

  YAML::Node* PDeclare::toYaml() {
    auto root = new YAML::Node();
    for (int i = 0; i < names.length(); i++) {
      (*root)["declare"]["names"].push_back(names[i]);
    }
    (*root)["declare"]["value"] = *expression->toYaml();
    return root;
  }

  void PDeclare::generateStatement(GScope* scope,
                                   GInstructionVector& instr) {
    debug("  declaring...")
    auto value = expression->generateExpression(scope, instr);
    if (names.length() == 1) {
      // handle the singular case
      auto newVar = scope->addObject(names[0], value->type);
      instr.push_back(GInstruction { SET, new GOPARG[2] {
            {value->registerNum}, {newVar->registerNum}
      }});
    } else {
      // handle the tuple case
      if (!isTupleType(value->type)) {
        throw ParserException("Unable to declare multiple values with a non-tuple return type.");
      }
      auto tupleTypes = value->type->subTypes;
      if (tupleTypes.length() != names.length()) {
        throw ParserException("The number of return values in the tuple do not match the number of identifiers in a declaration");
      }

      auto intVar = scope->allocateObject(getInt32Type());
      for (int i = 0; i < names.length(); i++) {
        auto newVar = scope->addObject(names[i], tupleTypes[i]);
        instr.push_back(GInstruction { LOAD_CONSTANT_INT, new GOPARG[2] {
              {intVar->registerNum}, {i}
        }});
        instr.push_back(GInstruction { ARRAY_LOAD_VALUE, new GOPARG[3] {
              {value->registerNum}, {intVar->registerNum}, {newVar->registerNum}
        }});
      }
    }
  }
}
