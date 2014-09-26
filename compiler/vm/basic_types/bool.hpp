#include "../object.hpp"

#ifndef VM_HPP_BASIC_TYPES_BOOL
#define VM_HPP_BASIC_TYPES_BOOL

namespace VM {

  VMClass* getVMBoolClass();

  bool evaluate_vmbool(VMObject* b);

  class VMBool : public VMObject {
  public:
    bool value;
    virtual VMClass* getType() { return getVMBoolClass(); }
    VMBool(bool _value): value(_value) {}
  };
}

#endif
