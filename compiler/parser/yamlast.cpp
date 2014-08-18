#include "yamlast.hpp"
#include "../lexer/tokens.hpp"
#include <cstdio>

using namespace lexer;

YAML::Node* YamlParseError(const char *str) { printf("Error: %s\n", str); return 0; }


YAML::Node* YamlAST::generateTree(NBlock& n) {
  YAML::Node* root = new YAML::Node();
  (*root)["main"]["block"] = *generate(n);
  return root;
}

YAML::Node* YamlAST::generate(Node& n) {
  return YamlParseError("type resulted to default node type.");
}

YAML::Node* YamlAST::generate(NExpression& n) {
  if (typeid(n) == typeid(NIdentifier)) {
    return generate(static_cast<NIdentifier&>(n));

  } else if (typeid(n) == typeid(NInteger)) {
    return generate(static_cast<NInteger&>(n));

  } else if (typeid(n) == typeid(NDouble)) {
    return generate(static_cast<NDouble&>(n));

  } else if (typeid(n) == typeid(NString)) {
    return generate(static_cast<NString&>(n));

  } else if (typeid(n) == typeid(NVoid)) {
    return generate(static_cast<NVoid&>(n));

  } else if (typeid(n) == typeid(NBoolean)) {
    return generate(static_cast<NBoolean&>(n));

  } else if (typeid(n) == typeid(NMethodCall)) {
    return generate(static_cast<NMethodCall&>(n));

  } else if (typeid(n) == typeid(NBinaryOperator)) {
    return generate(static_cast<NBinaryOperator&>(n));

  } else if (typeid(n) == typeid(NBlock)) {
    return generate(static_cast<NBlock&>(n));

  } else if (typeid(n) == typeid(NArray)) {
    return generate(static_cast<NArray&>(n));

  }

  return YamlParseError("Unable to find type for expression node!");
}

YAML::Node* YamlAST::generate(NArray& n) {
  return new YAML::Node("array");
}

YAML::Node* YamlAST::generate(NInteger& n) {
  YAML::Node* root = new YAML::Node();
  (*root) = n.value;
  return root;
}

YAML::Node* YamlAST::generate(NDouble& n) {
  YAML::Node* root = new YAML::Node();
  (*root) = n.value;
  return root;
}

YAML::Node* YamlAST::generate(NString& n) {
  YAML::Node* root = new YAML::Node();
  (*root) = n.value;
  return root;
}

YAML::Node* YamlAST::generate(NVoid& n) {
  YAML::Node* root = new YAML::Node();
  (*root) = "null";
  return root;
}

YAML::Node* YamlAST::generate(NBoolean& n) {
  YAML::Node* root = new YAML::Node();
  (*root) = n.value;
  return root;
}

YAML::Node* YamlAST::generate(NIdentifier& n) {
  YAML::Node* root = new YAML::Node();
  (*root) = n.name.c_str();
  return root;
}

YAML::Node* YamlAST::generate(NType& n) {
  YAML::Node* root = new YAML::Node();

  if (typeid(n) == typeid(NSingleType)) {
    NSingleType& singleType = static_cast<NSingleType&>(n);
    (*root) = singleType.name;

  } else if (typeid(n) == typeid(NArrayType)) {
    NArrayType& arrayType = static_cast<NArrayType&>(n);
    (*root) = "[" + arrayType.type.name + "]";

  }

  return root;
}


YAML::Node* YamlAST::generate(NMethodCall& n) {
  YAML::Node* root = new YAML::Node();
  (*root)["call"]["name"] = n.id.name.c_str();
  for (unsigned i = 0, e = n.arguments.size(); i != e; ++i) {
    (*root)["call"]["arguments"].push_back(*generate(*n.arguments[i]));
  }
  return root;
}

YAML::Node* YamlAST::generate(NBinaryOperator& n) {
  YAML::Node* root = new YAML::Node();
  std::string symbol;
  switch(n.op) {
  case PLUS:  symbol = "+"; break;
  case MINUS: symbol = "-"; break;
  case MUL:   symbol = "*"; break;
  case DIV:   symbol = "/"; break;
  case EQUAL:   symbol = "=="; break;
  case NOT_EQUAL:   symbol = "!="; break;
  case IS:    symbol = "is"; break;
  default:     return YamlParseError("invalid binary operator!");
  }
  (*root)["binary_operator"]["operator"] = symbol;
  (*root)["binary_operator"]["left"] = *generate(n.lhs);
  (*root)["binary_operator"]["right"] = *generate(n.rhs);
  return root;
}

YAML::Node* YamlAST::generate(NAssignment& n) {
  YAML::Node* root = new YAML::Node();
  (*root) = "assignment";
  return root;
}

YAML::Node* YamlAST::generate(NBlock& n) {
  YAML::Node* root = new YAML::Node();
  for (StatementList::iterator it = n.statements.begin(); it != n.statements.end(); it++) {
    root->push_back(*generate(**it));
  }
  return root;
}

YAML::Node* YamlAST::generate(NStatement& n) {

  if (typeid(n) == typeid(NConditional)) {
    return generate(static_cast<NConditional&>(n));

  } else if (typeid(n) == typeid(NReturn)) {
    return generate(static_cast<NReturn&>(n));

  } else if (typeid(n) == typeid(NAssignment)) {
    return generate(static_cast<NAssignment&>(n));

  } else if (typeid(n) == typeid(NVariableDeclaration)) {
    return generate(static_cast<NVariableDeclaration&>(n));

  } else if (typeid(n) == typeid(NFunctionDeclaration)) {
    return generate(static_cast<NFunctionDeclaration&>(n));
  }

  // expressions are also statement, so we default
  // to expression
  return generate(static_cast<NExpression&>(n));
}

YAML::Node* YamlAST::generate(NConditional& n) {
  YAML::Node* yaml = new YAML::Node();
  (*yaml)["conditional"]["condition_expression"] = *generate(n.condition);
  (*yaml)["conditional"]["then"] = *generate(n.ifBlock);
  (*yaml)["conditional"]["else"] = *generate(n.elseBlock);
 return yaml;
}

YAML::Node* YamlAST::generate(NReturn& n) {
  YAML::Node* yaml = new YAML::Node();
  (*yaml)["return"] = *generate(n.returnExpr);
  return yaml;
}

YAML::Node* YamlAST::generate(NVariableDeclaration& n) {
  YAML::Node* yaml = new YAML::Node();
  (*yaml)["variable_declaration"]["type"] = *generate(n.type);
  (*yaml)["variable_declaration"]["name"] = *generate(n.id);
  if (n.assignmentExpr != NULL) {
    (*yaml)["variable_declaration"]["assignment"] = *generate(*n.assignmentExpr);
  }
  return yaml;
}

YAML::Node* YamlAST::generate(NFunctionDeclaration& n) {
  YAML::Node* yaml = new YAML::Node();
  (*yaml)["function_declaration"]["name"] = n.id.name.c_str();
  for (unsigned i = 0, e = n.arguments.size(); i != e; ++i) {
    (*yaml)["function_declaration"]["arguments"].push_back(*generate(*n.arguments[i]));
  }
  (*yaml)["function_declaration"]["body"] = *(generate(n.block));
  return yaml;
}
