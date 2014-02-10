// ast methods
#include <cstdio>
#include <sstream>
#include "node.hpp"

using namespace std;

/* YAML::Node Node::generateAST() {
  YAML::Node node;
  node = this->nodeName();
  return node;
  } */

/* YAML::Node NReturn::generateAST() {
  YAML::Node node;
  NStatement::generateAST();
  returnExpr->generateAST();
}

YAML::Node NBlock::generateAST() {
  Node::generateAST();
  // print out child nodes
  for (StatementList::const_iterator it = statements.begin(); it != statements.end(); it++) {
    (**it).generateAST();
  }
}

YAML::Node NConditional::generateAST() {
  Node::generateAST();
  // print if statements
  condition.generateAST();
  ifBlock.generateAST();
  elseBlock.generateAST();
}

YAML::Node NBinaryOperator::generateAST() {
  NExpression::generateAST();
  lhs.generateAST();
  rhs.generateAST();
}

YAML::Node NFunctionDeclaration::generateAST() {
  NStatement::generateAST();
  block.generateAST();
}

YAML::Node NVariableDeclaration::generateAST() {
  NStatement::generateAST();
  if (assignmentExpr != NULL) {
    assignmentExpr->generateAST();
  }
} */
