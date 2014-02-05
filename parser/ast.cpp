// ast methods
#include <cstdio>
#include <sstream>
#include "node.hpp"

using namespace std;

string Node::nodeName() { return "Node"; }

string NInteger::nodeName() { 
  ostringstream output;
  output << "Integer: " << value;
  return output.str();
}

string NDouble::nodeName() { 
  return "Double"; 
}

string NIdentifier::nodeName() { 
  ostringstream output;
  output << "Indentifier: " << name;
  return output.str();
}

string NBoolean::nodeName() {
  return value ? "Boolean: true" : "Boolean: false";
}

string NMethodCall::nodeName() { return "Method Call"; }

string NBinaryOperator::nodeName() { 
  ostringstream output;
  output << "Binary Operator: " << op;
  return output.str();
}

string NAssignment::nodeName() { return "Assignment"; }

string NReturn::nodeName() { return "Return"; }

string NBlock::nodeName() { return "Block"; }

string NConditional::nodeName() { return "If Statement"; }

string NExpressionStatement::nodeName() { return "Expression"; }

string NVariableDeclaration::nodeName() { 
  ostringstream output;
  output << "Variable Declaration: " << type.name << " " << id.name;
  return output.str();
}

string NFunctionDeclaration::nodeName() { 
  ostringstream output;
  output << "Function Declaration: " << id.name;
  return output.str();
}

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
