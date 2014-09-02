#include "./object.hpp"
#include "./scope.hpp"

#ifndef VM_BUILTINS_HPP
#define VM_BUILTINS_HPP

namespace VM {

  extern VMScope* _BUILTIN_SCOPE;

  void vm_print(std::vector<VMObject*>& objects);
  VMMethod& _getVMPrint();
  VMScope& getBuiltinScope();

};

#endif
