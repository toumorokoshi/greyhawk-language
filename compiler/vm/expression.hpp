#include "exceptions.hpp"
#include "object.hpp"
#include "scope.hpp"
#include "statement.hpp"

#ifndef VM_EXPRESSION_HPP
#define VM_EXPRESSION_HPP

namespace VM {

  class VMExpression : public VMStatement {
  public:
    virtual VMObject* evaluate(VMScope& scope) = 0;
    virtual void execute(VMScope& scope) { evaluate(scope); }
    virtual ~VMExpression() {}
  };

  class VMConstant : public VMExpression {
  public:
    VMObject* object;
    virtual VMObject* evaluate(VMScope& scope) { return object; }
    VMConstant(VMObject* _object) : object(_object) {}
  };

  class VMIdentifier: public VMExpression {
  public:
    std::string identifier;
    virtual VMObject* evaluate(VMScope& scope) { return scope.getObject(identifier); }
    VMIdentifier(std::string _identifier) : identifier(_identifier) {}
  };

  class VMCall : public VMExpression {
  public:
    std::string methodName;
    std::vector<VMExpression*>& arguments;

    virtual VMObject* evaluate(VMScope& scope) {
      std::vector<VMObject*> evaluatedArguments;
      for (auto argument : arguments) {
        evaluatedArguments.push_back(argument->evaluate(scope));
      }

      return scope.invokeMethod(methodName, evaluatedArguments);
    }

    VMCall(std::string _methodName,
           std::vector<VMExpression*>& _arguments) :
      methodName(_methodName), arguments(_arguments) {}
  };

  class VMCallMethod : public VMExpression {
  public:
    VMMethod* method;
    VMObject* self;
    std::vector<VMExpression*>& arguments;
    VMCallMethod(VMObject* self, std::string methodName,
                 std::vector<VMExpression*>& _arguments) :
      arguments(_arguments) {

      VMClass* type = self->getType();
      if (type->methods.find(methodName) == type->methods.end()) {
        throw VMException("method " + methodName + " does not exist");
      }

      method = type->methods[methodName];
    }

    virtual VMObject* evaluate(VMScope& scope) {
      std::vector<VMObject*> evaluatedArguments;
      for (auto argument : arguments) {
        evaluatedArguments.push_back(argument->evaluate(scope));
      }

      return method->call(self, evaluatedArguments);
    }

  };
}

#endif
