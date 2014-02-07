#include <iostream>
#include "codegen.h"
#include "node.hpp"
extern NBlock* programBlock;
extern int yyparse();

int main(int argc, char **argv) {
  yyparse();

  CodeGenContext context;
  // context.printAST(*programBlock);
  context.generateCode(*programBlock);
  // context.runCode();
  return 0;
}
