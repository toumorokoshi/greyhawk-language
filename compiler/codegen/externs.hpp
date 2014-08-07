#include "llvm.h"

namespace codegen {

  void addExterns(llvm::Module&, llvm::ExecutionEngine&);

  // generate the put(string) instruction
  llvm::Function* generatePuts(llvm::Module&, llvm::ExecutionEngine&);

  // generate the print(string) instruction
  llvm::Function* generatePrint(llvm::Module&, llvm::ExecutionEngine&);
}
