#include "./class.hpp"

#ifndef VM_INTERFACE_HPP
#define VM_INTERFACE_HPP

namespace VM {

  class VMInterface : public VMClass {
  public:
    virtual bool matches(VMClass* otherClass) { return true; }
    VMInterface(std::string _name) : VMClass(_name) {}
  };

  VMInterface* getVMIStringable();
}

#endif
