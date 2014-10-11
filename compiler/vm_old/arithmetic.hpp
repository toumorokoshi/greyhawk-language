#include "./object.hpp"
#include "./scope.hpp"

#ifndef VM_ARITHMETIC_HPP
#define VM_ARITHMETIC_HPP


namespace VM {

  VMObject* vm_add(std::vector<VMObject*>& objects);
  VMFunction& _getVMAdd();

  VMObject* vm_sub(std::vector<VMObject*>& objects);
  VMFunction& _getVMSub();

  VMObject* vm_mul(std::vector<VMObject*>& objects);
  VMFunction& _getVMMul();

  VMObject* vm_div(std::vector<VMObject*>& objects);
  VMFunction& _getVMDiv();
};

#endif
