#include "file.hpp"
#include "../function.hpp"
#include <iostream>

namespace VM {

  VMClass* getVMFileClass() {
    auto static VMFileClass = new VMClass("File");
    return VMFileClass;
  }

  VMObject* _getVMStdin() {
    auto static vmStdin = new VMFile(std::cin);
    return vmStdin;
  }

}
