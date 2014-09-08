#include "./object.hpp"
#include "./scope.hpp"

#ifndef VM_ARITHMETIC_HPP
#define VM_ARITHMETIC_HPP


namespace VM {

  VMObject* vm_add(std::vector<VMObject*>& objects);
  VMObject* vm_sub(std::vector<VMObject*>& objects);
  VMObject* vm_mul(std::vector<VMObject*>& objects);
  VMObject* vm_div(std::vector<VMObject*>& objects);
};

#endif
