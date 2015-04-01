#include "node.hpp"
#include "yaml-cpp/yaml.h"

class YamlAST {
public:
  YamlAST() {}
  YAML::Node* generateTree(NBlock&);

  YAML::Node* root;

  // value configuration
  static YAML::Node* generate(Node& n);
  static YAML::Node* generate(NExpression& n);
  static YAML::Node* generate(NArray& n);
  static YAML::Node* generate(NInteger& n);
  static YAML::Node* generate(NDouble& n);
  static YAML::Node* generate(NString& n);
  static YAML::Node* generate(NVoid& n);
  static YAML::Node* generate(NBoolean& n);
  static YAML::Node* generate(NIdentifier& n);
  static YAML::Node* generate(NType& n);
  static YAML::Node* generate(NMethodCall& n);
  static YAML::Node* generate(NBinaryOperator& n);
  static YAML::Node* generate(NAssignment& n);
  static YAML::Node* generate(NBlock& n);
  static YAML::Node* generate(NClassInstantiation& n);

  // statements
  static YAML::Node* generate(NStatement& n);
  static YAML::Node* generate(NConditional& n);
  static YAML::Node* generate(NReturn& n);
  static YAML::Node* generate(NVariableDeclaration& n);
  static YAML::Node* generate(NFunctionDeclaration& n);
  static YAML::Node* generate(NClassDeclaration& n);
};
