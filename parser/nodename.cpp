/* file to contain all nodeName() methods */

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
