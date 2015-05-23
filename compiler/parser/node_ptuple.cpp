#include "nodes.hpp"
#include "parser.hpp"

using namespace VM;
using codegen::GScope;
using gstd::Array;
using lexer::L;

namespace parser {

  YAML::Node* PTuple::toYaml() {
    auto root = new YAML::Node();
    for (auto& value : values) {
      (*root)["tuple"].push_back(*value->toYaml());
    }
    return root;
  }

  GType* PTuple::getType(GScope* scope) {
    Array<GType*> types(values.size());
    for (int i = 0; i < types.length(); i++) {
      types[i] = values[i]->getType(scope);
    }
    return getTupleType(types);
  }

  GIndex* PTuple::generateExpression(GScope* scope,
                                     GInstructionVector& instr) {
    auto size = scope->allocateObject(getInt32Type());
    instr.push_back(GInstruction { LOAD_CONSTANT_INT, new GOPARG[2] {
          { size->registerNum } , { (int) values.size() }
    }});
    auto tuple = scope->allocateObject(getType(scope));
    instr.push_back(GInstruction { ARRAY_ALLOCATE, new GOPARG[2] {
          { tuple->registerNum }, { size->registerNum }
    }});
    auto intVar = scope->allocateObject(getInt32Type());
    for (int i = 0; i < (int) values.size(); i++) {
      auto value = values[i]->generateExpression(scope, instr);
      instr.push_back(GInstruction { LOAD_CONSTANT_INT, new GOPARG[2] {
            {intVar->registerNum}, {i}
      }});
      instr.push_back(GInstruction { ARRAY_SET_VALUE, new GOPARG[3] {
            { tuple->registerNum } , { intVar->registerNum }, { value->registerNum }
      }});
    }
    return tuple;
  }

  PTuple* Parser::parseTuple() {
    _validateToken(L::LPAREN, "expected a '(' for a tuple object");
    token_position++;
  }
};
