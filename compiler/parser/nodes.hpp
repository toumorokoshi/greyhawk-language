#include <map>
#include <string>
#include <vector>
#include "../lexer/tokens.hpp"
#include "../vm/vm.hpp"
#include "yaml-cpp/yaml.h"

#ifndef PARSER_NODES_HPP
#define PARSER_NODES_HPP

namespace parser {

  class PNode {
  public:
    virtual YAML::Node* toYaml() = 0;
    virtual ~PNode() {};
  };

  /* Statements */

  class PBlock;

  class PStatement : public PNode {
  public:
    virtual VM::VMStatement* generateStatement(VM::VMScope*) = 0;
  };

  typedef std::vector<PStatement*> PStatements;

  class PExpression : public PStatement {
  public:

    virtual VM::VMStatement* generateStatement(VM::VMScope* s) {
      return generateExpression(s);
    }

    virtual VM::VMExpression* generateExpression(VM::VMScope*) = 0;
    virtual VM::VMClass* getType(VM::VMScope*) = 0;
  };

  typedef std::vector<PExpression*> PExpressions;

  class PAssign : public PStatement {
  public:
    std::string name;
    PExpression* expression;

    virtual YAML::Node* toYaml();
    virtual VM::VMStatement* generateStatement(VM::VMScope*);

    PAssign(std::string _name,
            PExpression* _expression) :
      name(_name), expression(_expression) {}
  };

  class PDeclare : public PStatement {
  public:
    std::string name;
    PExpression* expression;

    virtual YAML::Node* toYaml();
    virtual VM::VMStatement* generateStatement(VM::VMScope*);

    PDeclare(std::string _name,
             PExpression* _expression) :
      name(_name), expression(_expression) {}
  };

  class PForLoop : public PStatement {
  public:
    std::string variableName;
    PExpression* iterableExpression;
    PBlock* block;

    virtual YAML::Node* toYaml();
    virtual VM::VMStatement* generateStatement(VM::VMScope*);

    PForLoop(std::string _variableName,
             PExpression* _iterableExpression,
             PBlock* _block) :
      variableName(_variableName),
      iterableExpression(_iterableExpression),
      block(_block) {}
  };

  typedef std::pair<std::string, std::string> PArgumentDefinition;
  typedef std::vector<PArgumentDefinition*> PArgumentList;

  class PFunctionDeclaration : public PStatement {
  public:
    std::string returnType;
    std::string name;
    PArgumentList& arguments;
    PBlock* body;

    virtual YAML::Node* toYaml();
    virtual VM::VMStatement* generateStatement(VM::VMScope*);

    PFunctionDeclaration(std::string _returnType,
                         std::string _name,
                         PArgumentList& _arguments,
                         PBlock* _body) :
      returnType(_returnType), name(_name),
      arguments(_arguments), body(_body) {}
  };

  class PIfElse : public PStatement {
  public:
    PExpression* condition;
    PBlock* trueBlock;
    PBlock* falseBlock;

    virtual YAML::Node* toYaml();
    virtual VM::VMStatement* generateStatement(VM::VMScope*) { return NULL; }

    PIfElse(PExpression* _condition,
            PBlock* _trueBlock,
            PBlock* _falseBlock):
      condition(_condition), trueBlock(_trueBlock),
      falseBlock(_falseBlock) {}
  };

  class PReturn : public PStatement {
  public:
    PExpression* expression;

    virtual YAML::Node* toYaml();
    virtual VM::VMStatement* generateStatement(VM::VMScope*);

    PReturn(PExpression* _expression) :
      expression(_expression) {}
  };

  /* expressions */

  class PConstantBool : public PExpression {
  public:
    bool value;

    virtual YAML::Node* toYaml();
    virtual VM::VMClass* getType(VM::VMScope*) { return VM::getVMBoolClass(); }
    virtual VM::VMExpression* generateExpression(VM::VMScope*) {
      return new VM::VMBool(value);
    }

    PConstantBool(bool _value) : value(_value) {}
  };

  class PConstantInt : public PExpression {
  public:
    int value;

    virtual YAML::Node* toYaml();
    virtual VM::VMClass* getType(VM::VMScope*) { return VM::getVMIntClass(); }
    virtual VM::VMExpression* generateExpression(VM::VMScope*) {
      return new VM::VMInt(value);
    }

    PConstantInt(int _value) : value(_value) {}
  };

  class PConstantString : public PExpression {
  public:
    std::string value;

    virtual YAML::Node* toYaml();
    virtual VM::VMClass* getType(VM::VMScope*) { return VM::getVMStringClass(); }
    virtual VM::VMExpression* generateExpression(VM::VMScope*) {
      return new VM::VMString(value);
    }

    PConstantString(std::string _value) : value(_value) {};
  };

  class PIdentifier : public PExpression {
  public:
    std::string name;

    virtual YAML::Node* toYaml();
    virtual VM::VMClass* getType(VM::VMScope* scope) {
      return scope->getObjectType(name);
    }

    virtual VM::VMExpression* generateExpression(VM::VMScope*) {
      return new VM::VMIdentifier(name);
    }

    PIdentifier(std::string _name) : name(_name) {}
  };

  class PFunctionCall : public PExpression {
  public:
    std::string name;
    PExpressions& arguments;

    virtual YAML::Node* toYaml();
    virtual VM::VMClass* getType(VM::VMScope*);
    virtual VM::VMExpression* generateExpression(VM::VMScope*);

    PFunctionCall(std::string _name,
                  PExpressions& _arguments) :
      name(_name), arguments(_arguments) {}
  };

  class PMethodCall : public PExpression {
  public:
    PExpression* currentValue;
    std::string methodName;
    PExpressions& arguments;

    virtual YAML::Node* toYaml();
    virtual VM::VMClass* getType(VM::VMScope*) { return NULL; }
    virtual VM::VMExpression* generateExpression(VM::VMScope*) { return NULL; }

    PMethodCall(PExpression* _currentValue,
                std::string _methodName,
                PExpressions& _arguments) :
      currentValue(_currentValue),
      methodName(_methodName),
      arguments(_arguments) {}
  };

  class PBinaryOperation : public PExpression {
  public:
    PExpression* lhs;
    lexer::L op;
    PExpression* rhs;

    virtual YAML::Node* toYaml();
    virtual VM::VMClass* getType(VM::VMScope* s) { return lhs->getType(s); }
    virtual VM::VMExpression* generateExpression(VM::VMScope*);

    PBinaryOperation(PExpression* _lhs,
                     lexer::L _op,
                     PExpression* _rhs) :
      lhs(_lhs), op(_op), rhs(_rhs) {}
  };

  /* block */

  class PBlock : public PNode {
  public:
    PStatements statements;
    virtual YAML::Node* toYaml();
    VM::VMBlock* generate(VM::VMScope*);
  };
}

#endif
