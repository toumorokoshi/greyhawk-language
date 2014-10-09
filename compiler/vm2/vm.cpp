#include "jit.hpp"

using namespace VM;

void initializeLLVM() {
  llvm::InitializeNativeTargetAsmPrinter();
  llvm::InitializeNativeTargetAsmParser();
}



int main(int argc, char const *argv[]) {

  auto jit = new JIT();

  // our return value
  auto arguments = new GValue[1] {
    GValue { new GType { BASICTYPES::INT32, "Int"},
             new int(10)
    }
  };

  auto instructions = new GInstruction {
   GOPCODE::RETURN,
   arguments
  };

  auto returnFunction = new GFunction {
    new GType { BASICTYPES::NONE, "None" },
    instructions, 1
  };

  jit->compile(returnFunction);
}
