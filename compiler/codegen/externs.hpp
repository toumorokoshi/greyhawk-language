#include "llvm.h"

namespace codegen {

  void addExterns(llvm::Module&);

  // generate the put(string) instruction
  llvm::Function* generatePuts(llvm::Module&);
}
