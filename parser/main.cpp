#include <iostream>
#include "codegenerator.hpp"
#include "node.hpp"
extern NBlock* programBlock;
extern int yyparse();

int main(int argc, char **argv) {
  yyparse();

  CodeGenerator generator;
  // context.printAST(*programBlock);
  generator.generateCode(*programBlock);
  // context.runCode();
  return 0;
}
