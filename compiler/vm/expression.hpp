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


  class VMAttribute : public VMExpression {
  public:
    VMExpression& parent;
    std::string child;
    VMAttribute(VMExpression _parent, std::string _child) :
      parent(_parent), child(_child) {}

    virtual VMObject* evaluate(VMScope& scope) {
      auto parentObject = parent.evaluate(scope);
      return parentObject->call(child, new std::vector<VMObject*>());
    }

  };

  class VMCall : public VMExpression {
  public:
    VMExpression& methodExpression;
    std::vector<VMExpression*>& arguments;

    virtual VMObject* evaluate(VMScope& scope) {
      std::vector<VMObject*> evaluatedArguments;
      for (auto argument : arguments) {
        evaluatedArguments.push_back(argument->evaluate(scope));
      }

      auto method = (VMFunction*) methodExpression.evaluate(scope);
      return method->call(evaluatedArguments);
    }

    VMCall(VMExpression& _methodExpression,
           std::vector<VMExpression*>& _arguments) :
      methodExpression(_methodExpression), arguments(_arguments) {}
  };

  class VMCallMethod : public VMExpression {
  public:
    VMExpression* selfExpression;
    std::string methodName;
    std::vector<VMExpression*>& arguments;

    VMCallMethod(VMExpression* _selfExpression,
                 std::string _methodExpression,
                 std::vector<VMExpression*>& _arguments) :
      selfExpression(_selfExpression),
      methodExpression(_methodExpression),
      arguments(_arguments)
    {}

    virtual VMObject* evaluate(VMScope& scope) {
      std::vector<VMObject*> evaluatedArguments;
      for (auto argument : arguments) {
        evaluatedArguments.push_back(argument->evaluate(scope));
      }

      auto self = selfExpression->evaluate(scope);

      return self->call(methodName, evaluatedArguments);
    }

  };
}

#endif
