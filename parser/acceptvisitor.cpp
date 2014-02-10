// methods to accept a visitor
#include "node.hpp"

void Node::acceptVisitor(class Visitor &v) {
  v.visit(this);
}

void NInteger::acceptVisitor(class Visitor &v) {
  v.visit(this);
}

void NDouble::acceptVisitor(class Visitor &v) {
  v.visit(this);
}

void NIdentifier::acceptVisitor(class Visitor &v) {
  v.visit(this);
}

void NBoolean::acceptVisitor(class Visitor &v) {
  v.visit(this);
}

void NMethodCall::acceptVisitor(class Visitor &v) {
  v.visit(this);
}

/* void NBinaryOperator::acceptVisitor(class Visitor &v) {
  v.visit(this);
  v.visit(lhs);
  v.visit(lhs);
  v.visit(this);
  } */
