#include "./object.hpp"
#include "./scope.hpp"

#ifndef VM_BUILTINS_HPP
#define VM_BUILTINS_HPP

namespace VM {

  VMObject* vm_print(std::vector<VMObject*>& objects);
  VMFunction& _getVMPrint();
  VMScope& getBuiltinScope();

};

#endif
