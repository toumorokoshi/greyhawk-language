// ast methods
#include <cstdio>
#include "node.h"

using namespace std;

string Node::nodeName() { return "Node"; }
string NInteger::nodeName() { return "Integer"; }
string NDouble::nodeName() { return "Double"; }
string NIdentifier::nodeName() { return "Identifier"; }
string NMethodCall::nodeName() { return "Method Call"; }
string NBinaryOperator::nodeName() { return "Binary Operator"; }
string NAssignment::nodeName() { return "Assignment"; }
string NBlock::nodeName() { return "Block"; }
string NExpressionStatement::nodeName() { return "Expression"; }
string NVariableDeclaration::nodeName() { return "Variable Declaration"; }
string NFunctionDeclaration::nodeName() { return "Function Declaration"; }

void Node::printAST(int indentation) {
  cout << this->nodeName() << endl;
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
