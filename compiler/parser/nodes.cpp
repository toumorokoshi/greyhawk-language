#include "nodes.hpp"

namespace parser {

  YAML::Node* PBlock::toYaml() {
    auto root = new YAML::Node();
    for (auto statement: statements) {
      root->push_back(*statement->toYaml());
    }
    return root;
  }

  YAML::Node* PAssign::toYaml() {
    auto root = new YAML::Node();
    (*root)["assign"]["name"] = name;
    (*root)["assign"]["value"] = *expression->toYaml();
    return root;
  }

  YAML::Node* PDeclare::toYaml() {
    auto root = new YAML::Node();
    (*root)["declare"]["name"] = name;
    (*root)["declare"]["value"] = *expression->toYaml();
    return root;
  }

  YAML::Node* PForLoop::toYaml() {
    auto root = new YAML::Node();
    (*root)["for_loop"]["variable_name"] = variableName;
    (*root)["for_loop"]["iterable"] = *iterableExpression->toYaml();
    (*root)["for_loop"]["block"] = *block->toYaml();
    return root;
  }

  YAML::Node* PFunctionDeclaration::toYaml() {
    auto root = new YAML::Node();
    (*root)["function_declaration"]["return_type"] = returnType;
    (*root)["function_declaration"]["name"] = name;

    for (auto argument : arguments) {
      YAML::Node& argumentNode = *new YAML::Node();
      argumentNode["name"] = argument->first;
      argumentNode["type"] = argument->second;
      (*root)["function_declaration"]["arguments"].push_back(argumentNode);
    }

    (*root)["function_declaration"]["body"] = *body->toYaml();
    return root;
  }

  YAML::Node* PIfElse::toYaml() {
    auto root = new YAML::Node();
    (*root)["if_else"]["condition"] = *condition->toYaml();
    (*root)["if_else"]["true_block"] = *trueBlock->toYaml();
    (*root)["if_else"]["false_block"] = *falseBlock->toYaml();
    return root;
  }

  YAML::Node* PReturn::toYaml() {
    auto root = new YAML::Node();
    (*root)["return"] = *expression->toYaml();
    return root;
  }

  YAML::Node* PConstantBool::toYaml() {
    return new YAML::Node(value);
  }

  YAML::Node* PConstantInt::toYaml() {
    return new YAML::Node(value);
  }

  YAML::Node* PConstantString::toYaml() {
    auto node = new YAML::Node();
    (*node)["string"] = value;
    return node;
  }

  YAML::Node* PIdentifier::toYaml() {
    auto node = new YAML::Node();
    (*node)["identifier"] = name;
    return node;
  }

  YAML::Node* PArray::toYaml() {
    auto node = new YAML::Node();
    for (auto element: elements) {
      node->push_back(*element->toYaml());
    }
    return node;
  }

  YAML::Node* PFunctionCall::toYaml() {
    auto node = new YAML::Node();
    (*node)["function_call"]["name"] = name;
    for (auto argument : arguments) {
      (*node)["function_call"]["arguments"].push_back(*argument->toYaml());
    }
    return node;
  }

  YAML::Node* PMethodCall::toYaml() {
    auto node = new YAML::Node();
    (*node)["method_call"]["object"] = *currentValue->toYaml();
    (*node)["method_call"]["method_name"] = methodName;
    for (auto argument: arguments) {
      (*node)["method_call"]["arguments"].push_back(*argument->toYaml());
    }
    return node;
  }

  YAML::Node* PBinaryOperation::toYaml() {
    auto node = new YAML::Node();
    (*node)["binary_operation"]["operation"] = lexer::tokenMap[op];
    (*node)["binary_operation"]["lhs"] = *lhs->toYaml();
    (*node)["binary_operation"]["rhs"] = *rhs->toYaml();
    return node;
  }

}