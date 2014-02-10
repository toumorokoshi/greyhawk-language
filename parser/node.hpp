#include <iostream>
#include <vector>
#include <yaml-cpp/yaml.h>
#include "llvm.h"

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
class NExpressionDeclaration;
class NVariableDeclaration;
class NFunctionDeclaration;

typedef std::vector<NStatement*> StatementList;
typedef std::vector<NExpression*> ExpressionList;
typedef std::vector<NVariableDeclaration*> VariableList;


class Visitor {
public:
  virtual void visit(Node *n) = 0;
  virtual void visit(NExpression* n) = 0;
  virtual void visit(NDouble *n) = 0;
  virtual void visit(NIdentifier *n) = 0;
  virtual void visit(NBoolean *n) = 0;
  virtual void visit(NMethodCall *n) = 0;
  virtual void visit(NBinaryOperator *n) = 0;
  virtual void visit(NAssignment *n) = 0;
  virtual void visit(NBlock *n) = 0;
  virtual void visit(NStatement *n) = 0;
  virtual void visit(NConditional *n) = 0;
  virtual void visit(NReturn *n) = 0;
  virtual void visit(NExpressionDeclaration *n) = 0;
  virtual void visit(NVariableDeclaration *n) = 0;
  virtual void visit(NFunctionDeclaration *n) = 0;
};

class Node {
 public:
  virtual ~Node() {}
  virtual llvm::Value* codeGen(CodeGenContext& context);
  virtual std::string nodeName();
  // print the AST for this and all child nodes, at the particular indentation
  virtual void acceptVisitor(class Visitor &v);
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
  virtual llvm::Value* codeGen(CodeGenContext& context);
  virtual std::string nodeName();
  virtual void acceptVisitor(class Visitor &v);
};

class NDouble : public NExpression {
 public:
  double value;
  NDouble(double value) : value(value) { }
  virtual llvm::Value* codeGen(CodeGenContext& context);
  virtual std::string nodeName();
  virtual void acceptVisitor(class Visitor &v);
};

class NIdentifier : public NExpression {
 public:
  std::string name;
  NIdentifier(const std::string& name) : name(name) { }
  virtual llvm::Value* codeGen(CodeGenContext& context);
  virtual std::string nodeName();
  virtual void acceptVisitor(class Visitor &v);
};

class NBoolean : public NExpression {
 public:
  bool value;
  NBoolean(bool value) : value(value) { }
  virtual llvm::Value* codeGen(CodeGenContext& context);
  virtual std::string nodeName();
  virtual void acceptVisitor(class Visitor &v);
};

class NMethodCall : public NExpression {
 public:
  const NIdentifier& id;
  ExpressionList arguments;
  NMethodCall(const NIdentifier& id, ExpressionList& arguments) : 
    id(id), arguments(arguments) { }
  NMethodCall(const NIdentifier& id) :  id(id) { }
  virtual llvm::Value* codeGen(CodeGenContext& context);
  virtual std::string nodeName();
  virtual void acceptVisitor(class Visitor &v);
};

class NBinaryOperator : public NExpression {
 public:
  int op;
  NExpression& lhs;
  NExpression& rhs;
  NBinaryOperator(NExpression& lhs, int op, NExpression& rhs) : 
    lhs(lhs), rhs(rhs), op(op) { }
  virtual llvm::Value* codeGen(CodeGenContext& context);
  virtual std::string nodeName();
};

class NAssignment : public NExpression {
 public:
  NIdentifier& lhs;
  NExpression& rhs;
  NAssignment(NIdentifier& lhs, NExpression& rhs) : lhs(lhs), rhs(rhs) {}
  virtual llvm::Value* codeGen(CodeGenContext& context);
  virtual std::string nodeName();
};

class NBlock : public NExpression {
 public:
  StatementList statements;
  NBlock() { }
  virtual llvm::Value* codeGen(CodeGenContext& context);
  virtual std::string nodeName();
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
  virtual llvm::Value* codeGen(CodeGenContext& context);
  virtual std::string nodeName();
};

class NReturn: public NStatement {
 public:
  NExpression& returnExpr;
  NReturn(NExpression& returnExpr) : returnExpr(returnExpr) {}
  virtual llvm::Value* codeGen(CodeGenContext& context);
  virtual std::string nodeName();
};


class NExpressionStatement : public NStatement {
 public:
  NExpression* expression;
  NExpressionStatement(NExpression* expression) : expression(expression) { }
  virtual llvm::Value* codeGen(CodeGenContext& context);
  virtual std::string nodeName();
};

class NVariableDeclaration : public NStatement {
 public:
  const NIdentifier& type;
  NIdentifier& id;
  NExpression *assignmentExpr;
  NVariableDeclaration(const NIdentifier& type, NIdentifier& id) : type(type), id(id) { }
  NVariableDeclaration(const NIdentifier& type, NIdentifier& id, NExpression *assignmentExpr) : 
    type(type), id(id), assignmentExpr(assignmentExpr)  { }
  virtual llvm::Value* codeGen(CodeGenContext& context);
  virtual std::string nodeName();
};

class NFunctionDeclaration : public NStatement {
 public:
  const NIdentifier& type;
  const NIdentifier& id;
  VariableList arguments;
  NBlock& block;
  NFunctionDeclaration(const NIdentifier& type, const NIdentifier& id, const VariableList& arguments, NBlock& block) :
    type(type), id(id), arguments(arguments), block(block) { }
  virtual llvm::Value* codeGen(CodeGenContext& context);
  virtual std::string nodeName();
};
