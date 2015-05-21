#include "parser.hpp"

using namespace lexer;
using namespace codegen;
using namespace VM;

using gstd::Array;

namespace parser {

  GType* calculateType(GScope* scope, std::string name) {
    auto typeObject  = scope->getClass(name);
    if (typeObject == NULL) {
      typeObject = evaluateType(name);
    }
    return typeObject;
  }

  PType* Parser::parseType() {
    auto token = *token_position;
    token_position++;
    switch(token->type) {
    case TYPE:
      return new PSingleType(token->value);
      break;
    case LPAREN: {
      std::vector<PType*> types;
      token_position++;
      while ((*token_position)->type != RPAREN) {
        types.push_back(parseType());
        _validateToken(COMMA, "expected a comma in between tuple arguments");
      }
      token_position++;

      if (types.size() < 2) {
        throw ParserException("Expected at least two types for a tuple type declaration");
      }

      return new PTupleType(types);
    }
    default:
      throw ParserException("Expected type declaration");
    }
  }

  GType* PSingleType::generateType(GScope* scope) {
    return calculateType(scope, type);
  }

  YAML::Node* PSingleType::toYaml() {
    return new YAML::Node(type);
  }

  std::string PSingleType::getName() { return type; }

  GType* PTupleType::generateType(GScope* scope) {
    Array<VM::GType*> tupleTypes(new GType*[types.size()],
                                 types.size());
    for (int i = 0; i < types.size(); i++) {
      tupleTypes[i] = types[i]->generateType(scope);
    }
    return getTupleType(tupleTypes);
  }

  YAML::Node* PTupleType::toYaml() {
    auto root = new YAML::Node();
    for (auto& type : types) {
      (*root)["tuple"]["types"].push_back(type->getName());
    }
    return root;
  }

  std::string PTupleType::getName() {
    std::stringstream typeStream;
    typeStream << "Tuple<";
    for (int i = 0; i < types.size(); i++) {
      auto type = types[i]->getName();
      typeStream << type;
      if (i + 1 < types.size()) {
        typeStream << ",";
      }
    }
    typeStream << ">";
    return typeStream.str();
  };

}
