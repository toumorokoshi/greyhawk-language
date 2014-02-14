#include "node.hpp"
#include "build/parser.hpp"
#include "yaml-cpp/yaml.h"

class YamlAST {
public:
  YamlAST() {}
  YAML::Node* generateTree(NBlock&);

private:
  YAML::Node* root;

  // value configuration
//   YAML::Node* generate(Node& n);
   YAML::Node* generate(NExpression& n);
   YAML::Node* generate(NInteger& n);
   YAML::Node* generate(NDouble& n);
   YAML::Node* generate(NVoid& n);
   YAML::Node* generate(NBoolean& n);
   YAML::Node* generate(NIdentifier& n);
   YAML::Node* generate(NMethodCall& n);
   YAML::Node* generate(NBinaryOperator& n);
   YAML::Node* generate(NAssignment& n);
   YAML::Node* generate(NBlock& n);
   YAML::Node* generate(NStatement& n);
   YAML::Node* generate(NConditional& n);
   YAML::Node* generate(NReturn& n);
   YAML::Node* generate(NExpressionStatement& n);
   YAML::Node* generate(NVariableDeclaration& n);
   YAML::Node* generate(NFunctionDeclaration& n);
};
