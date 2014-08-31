#include "./class.hpp"

#ifndef VM_OBJECT_HPP
#define VM_OBJECT_HPP

namespace VM {

  class VMObject {
  public:
    VMClass* classInstance;
    std::map<std::string, VMObject&> attributes;
  };
}

#endif
