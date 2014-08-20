#include <iostream>
#include <vector>

#ifndef NODE_HPP

#define NODE_HPP

class CodeGenContext;

// nodes in the syntax tree
class Node;

// expressions + subclasses
class NExpression;
class NArray;
class NInteger;
class NDouble;
class NString;
class NVoid;
class NBoolean;
class NIdentifier;
class NType;
class NSingleType;
class NArrayType;
class NMethodCall;
class NBinaryOperator;
class NAssignment;
class NBlock;
class NClassInstantiation;

// statements + subclasses
class NStatement;
class NConditional;
class NReturn;
class NBlock;
class NVariableDeclaration;
class NFunctionDeclaration;
class NClassDeclaration;

typedef std::vector<Node*> NodeVector;
typedef std::vector<NStatement*> StatementList;
typedef std::vector<NExpression*> ExpressionList;
typedef std::vector<NVariableDeclaration*> VariableList;


class Node {
 public:
  virtual ~Node() {}
};

/*******************************************/
/*             STATEMENTS                  */

// Statements do not necessarily return a value
class NStatement : public Node {
public:
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


class NVariableDeclaration : public NStatement {
 public:
  NType& type;
  NIdentifier& id;
  NExpression* assignmentExpr;
  NVariableDeclaration(NIdentifier& id, NType& type) : type(type), id(id), assignmentExpr(NULL) { }
  NVariableDeclaration(NIdentifier& id, NType& type, NExpression *assignmentExpr) :
    type(type), id(id), assignmentExpr(assignmentExpr)  { }
};

class NAssignment : public NStatement {
 public:
  NIdentifier& lhs;
  NExpression& rhs;
  NAssignment(NIdentifier& lhs, NExpression& rhs) : lhs(lhs), rhs(rhs) {}
};

class NFunctionDeclaration : public NStatement {
 public:
  NType& type;
  NIdentifier& id;
  VariableList arguments;
  NBlock& block;
  NFunctionDeclaration(NType& type, NIdentifier& id, VariableList& arguments, NBlock& block) :
    type(type), id(id), arguments(arguments), block(block) { }
};

class NClassDeclaration: public NStatement {
public:
  NSingleType& name;
  VariableList& attributes;
  NClassDeclaration(NSingleType& _name, VariableList& _attributes) :
    name(_name), attributes(_attributes) {}
};

/*******************************************/
/*             EXPRESSIONS                 */

// expressions are nodes that always return a value, the value of the
// evaluated result of the expression.

class NExpression : public NStatement {
};

class NArray: public NExpression {
public:
  ExpressionList& elements;
  NArray(ExpressionList& _elements) :
    elements(_elements) {}
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

class NString: public NExpression {
public:
  const std::string value;
  NString(std::string value) : value(value) {}
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

class NType: public NExpression {
};

class NSingleType : public NType {
public:
  std::string name;
  NSingleType(const std::string& name) : name(name) { }
};

class NArrayType: public NType {
public:
  NSingleType & type;
  NArrayType(NSingleType & _type) : type(_type) {}
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
  NExpression& lhs;
  int op;
  NExpression& rhs;
  NBinaryOperator(NExpression& lhs, int op, NExpression& rhs) :
    lhs(lhs), op(op), rhs(rhs) { }
};


class NClassInstantiation : public NExpression {
public:
  NSingleType& type;
  ExpressionList& parameters;
  NClassInstantiation(NSingleType& _type,
                      ExpressionList& _parameters) :
    type(_type), parameters(_parameters) {}
};

class NBlock : public NExpression {
 public:
  StatementList statements;
  NBlock() { }
};

#endif
