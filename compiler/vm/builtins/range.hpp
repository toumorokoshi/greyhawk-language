#include "../class.hpp"
#include "../object.hpp"
#include "../string.hpp"

#ifndef VM_BUILTINS_RANGE_HPP
#define VM_BUILTINS_RANGE_HPP

namespace VM {

  VMObject* vm_range(std::vector<VMObject*>& objects);
  VMFunction& _getVMRange();

}

#endif
