#include "./executionengine.hpp"
#include "./exceptions.hpp"

namespace VM {

  void VMExecutionEngine::executeBlock(NBlock& block) {
    for (auto statement : block.statements) {
      executeStatement(statement);
    }
  }

  void VMExecutionEngine::executeStatement(NStatement* statement) {
    if (auto expression = dynamic_cast<NExpression*>(statement)) {
      executeExpression(expression);
    }
  }

  VMObject* VMExecutionEngine::executeExpression(NExpression* expression) {

    if (auto identifier = dynamic_cast<NIdentifier*>(expression)) {
      if (scope.locals.find(identifier->name) == scope.locals.end()) {
        throw VMException(identifier->name + " is not defined in the local scope!");
      }
      return &scope.locals[identifier->name];

    } else if (auto methodCall = dynamic_cast<NMethodCall*>(expression)) {
      std::vector<VMObject*> arguments;
      for (auto argument : methodCall->arguments) {
        arguments.push_back(executeExpression(argument));
      }
      return scope.invokeMethod(methodCall->id.name, arguments);

    }
  }
};
