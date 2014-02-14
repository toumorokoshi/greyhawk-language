#include <iostream>
#include "codegenerator.hpp"
#include "yamlast.hpp"
#include "node.hpp"
extern NBlock* programBlock;
extern int yyparse();

int main(int argc, char **argv) {
  yyparse();

  CodeGenerator generator;
  YamlAST astGenerator;
  // context.printAST(*programBlock);
  // generator.generateCode(*programBlock);
  YAML::Node* tree = astGenerator.generateTree(*programBlock);
  std::cout << (*tree) << std::endl;
  // context.runCode();
  return 0;
}
