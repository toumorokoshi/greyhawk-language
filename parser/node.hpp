#include <iostream>
#include <vector>
#include <yaml-cpp/yaml.h>
#include "llvm.h"

#ifndef NODE_HPP

#define NODE_HPP

class CodeGenContext;

// nodes in the syntax tree
class Node;

// expressions + subclasses
class NExpression;
class NInteger;
class NDouble;
class NVoid;
class NIdentifier;
class NBoolean;
class NMethodCall;
class NBinaryOperator;
class NAssignment;
class NBlock;

// statements + subclasses
class NStatement;
class NConditional;
class NReturn;
class NExpressionStatement;
class NVariableDeclaration;
class NFunctionDeclaration;

typedef std::vector<NStatement*> StatementList;
typedef std::vector<NExpression*> ExpressionList;
typedef std::vector<NVariableDeclaration*> VariableList;


class Node {
 public:
  virtual ~Node() {}
};

/*******************************************/
/*             EXPRESSIONS                 */

// expressions are nodes that always return a value, the value of the
// evaluated result of the expression.

class NExpression : public Node {
};

class NInteger : public NExpression {
 public:
  long long value;
  NInteger(long long value) : value(value) { }
};

class NDouble : public NExpression {
 public:
  double value;
  NDouble(double value) : value(value) { }
};

class NVoid : public NExpression {
public:
  NVoid() {}
};

class NIdentifier : public NExpression {
 public:
  std::string name;
  NIdentifier(const std::string& name) : name(name) { }
};

class NBoolean : public NExpression {
 public:
  bool value;
  NBoolean(bool value) : value(value) { }
};

class NMethodCall : public NExpression {
 public:
  NIdentifier& id;
  ExpressionList arguments;
  NMethodCall(NIdentifier& id, ExpressionList& arguments) : 
    id(id), arguments(arguments) { }
  NMethodCall(NIdentifier& id) :  id(id) { }
};

class NBinaryOperator : public NExpression {
 public:
  int op;
  NExpression& lhs;
  NExpression& rhs;
  NBinaryOperator(NExpression& lhs, int op, NExpression& rhs) : 
    lhs(lhs), rhs(rhs), op(op) { }
};

class NAssignment : public NExpression {
 public:
  NIdentifier& lhs;
  NExpression& rhs;
  NAssignment(NIdentifier& lhs, NExpression& rhs) : lhs(lhs), rhs(rhs) {}
};

class NBlock : public NExpression {
 public:
  StatementList statements;
  NBlock() { }
};

/*******************************************/
/*             STATEMENTS                  */

// Statements do not necessarily return a value
class NStatement : public Node {
};

class NConditional : public NStatement {
public:
  NExpression& condition;
  NBlock& ifBlock;
  NBlock& elseBlock;
  NConditional(NExpression& condition, NBlock& ifBlock, NBlock &elseBlock) : 
    condition(condition), ifBlock(ifBlock), elseBlock(elseBlock) {}
};

class NReturn: public NStatement {
 public:
  NExpression& returnExpr;
  NReturn(NExpression& returnExpr) : returnExpr(returnExpr) {}
};


class NExpressionStatement : public NStatement {
 public:
  NExpression& expression;
  NExpressionStatement(NExpression& expression) : expression(expression) { }
};

class NVariableDeclaration : public NStatement {
 public:
  NIdentifier& type;
  NIdentifier& id;
  NExpression *assignmentExpr;
  NVariableDeclaration(NIdentifier& type, NIdentifier& id) : type(type), id(id), assignmentExpr(NULL) { }
  NVariableDeclaration(NIdentifier& type, NIdentifier& id, NExpression *assignmentExpr) : 
    type(type), id(id), assignmentExpr(assignmentExpr)  { }
};

class NFunctionDeclaration : public NStatement {
 public:
  NIdentifier& type;
  NIdentifier& id;
  VariableList arguments;
  NBlock& block;
  NFunctionDeclaration(NIdentifier& type, NIdentifier& id, VariableList& arguments, NBlock& block) :
    type(type), id(id), arguments(arguments), block(block) { }
};

#endif
