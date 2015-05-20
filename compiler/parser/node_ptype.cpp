#include "parser.hpp"

using namespace lexer;
using namespace codegen;
using namespace VM;

#ifndef PARSER_PARSER_TYPE
#define PARSER_PARSER_TYPE

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
    switch(token->type) {
    case TYPE:
      return new PType(token->value);
      break;
    }
    return NULL;
  }

  GType* PType::generateType(GScope* scope) {
    return calculateType(scope, type);
  }

  YAML::Node* PType::toYaml() {
    return new YAML::Node(type);
  }

  std::string PType::getName() { return type; }

  GType* PTupleType::generateType(GScope* scope) {
    auto tupleTypes = new GType*[types.length()];
    std::stringstream typeStream;
    typeStream << "Tuple<";
    for (int i = 0; i < types.length(); i++) {
      auto type = types[i]->generateType(scope);
      tupleTypes[i] = type;
      typeStream << type->name;
      if (i + 1 < types.length()) {
        typeStream << ",";
      }
    }
    typeStream << ">";
    auto name = typeStream.str();

    return new GType {
      .name = name,
      .subTypes = tupleTypes
    };
  }

  YAML::Node* PTupleType::toYaml() {
    auto root = new YAML::Node();
    for (int i = 0; i < types.length(); i++) {
      (*root)["tuple"]["types"].push_back(types[i]->getName());
    }
    return root;
  }

  std::string PTupleType::getName() {
    std::stringstream typeStream;
    typeStream << "Tuple<";
    for (int i = 0; i < types.length(); i++) {
      auto type = types[i]->getName();
      typeStream << type;
      if (i + 1 < types.length()) {
        typeStream << ",";
      }
    }
    typeStream << ">";
    return typeStream.str();
  };

}

#endif
