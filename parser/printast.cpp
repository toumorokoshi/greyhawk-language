// ast methods
#include <cstdio>
#include <sstream>
#include "node.hpp"

using namespace std;

void Node::printAST(int indentation) {
  cout << string(indentation, '\t') << this->nodeName() << endl;
}

void NReturn::printAST(int indentation) {
  NStatement::printAST(indentation);
  indentation++;
  returnExpr->printAST(indentation);
  indentation--;
}

void NBlock::printAST(int indentation) {
  Node::printAST(indentation);
  // print out child nodes
  indentation++;
  for (StatementList::const_iterator it = statements.begin(); it != statements.end(); it++) {
    (**it).printAST(indentation);
  }
  indentation--;
}

void NConditional::printAST(int indentation) {
  Node::printAST(indentation);
  // print if statements
  indentation++;
  condition->printAST(indentation);
  ifBlock->printAST(indentation);
  elseBlock->printAST(indentation);
  indentation--;
}

void NBinaryOperator::printAST(int indentation) {
  NExpression::printAST(indentation);
  indentation++;
  lhs.printAST(indentation);
  rhs.printAST(indentation);
  indentation--;
}

void NFunctionDeclaration::printAST(int indentation) {
  NStatement::printAST(indentation);
  indentation++;
  block.printAST(indentation);
  indentation--;
}

void NVariableDeclaration::printAST(int indentation) {
  NStatement::printAST(indentation);
  indentation++;
  if (assignmentExpr != NULL) {
    assignmentExpr->printAST(indentation);
  }
  indentation--;
}
