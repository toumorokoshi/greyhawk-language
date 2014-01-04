// ast methods
#include <cstdio>
#include <sstream>
#include "node.h"

using namespace std;

string Node::nodeName() { return "Node"; }
string NInteger::nodeName() { return "Integer"; }
string NDouble::nodeName() { return "Double"; }

string NIdentifier::nodeName() { 
  ostringstream output;
  output << "Indentifier: " << name;
  return output.str();
}

string NMethodCall::nodeName() { return "Method Call"; }
string NBinaryOperator::nodeName() { return "Binary Operator"; }
string NAssignment::nodeName() { return "Assignment"; }
string NBlock::nodeName() { return "Block"; }
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

void NBlock::printAST(int indentation) {
  Node::printAST(indentation);
  // print out child nodes
  indentation++;
  for (StatementList::const_iterator it = statements.begin(); it != statements.end(); it++) {
    (**it).printAST(indentation);
  }
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
