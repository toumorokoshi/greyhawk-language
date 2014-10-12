#include "../object.hpp"
#include "../expression.hpp"

#ifndef VM_HPP_BASIC_TYPES_BOOL
#define VM_HPP_BASIC_TYPES_BOOL

namespace VM {

  VMClass* getVMBoolClass();

  bool evaluate_vmbool(VMObject* b);

  class VMBool : public VMObject, public VMExpression {
  public:
    bool value;

    virtual VMClass* getType() { return getVMBoolClass(); }
    virtual VMObject* evaluate(VMScope&) { return this; }

    VMBool(bool _value): value(_value) {}
  };
}

#endif
