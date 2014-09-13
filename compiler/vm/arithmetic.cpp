#include "./arithmetic.hpp"
#include <iostream>

namespace VM {

  VMObject* vm_add(std::vector<VMObject*>& objects) {
    auto left = dynamic_cast<VMInt*>(objects[0]);
    auto right = dynamic_cast<VMInt*>(objects[1]);
    return new VMInt(left->value + right->value);
  }

  VMFunction& _getVMAdd() {
    static auto argumentTypes = new std::vector<VMClass*> {
      getVMIntClass(), getVMIntClass()
    };
    static VMFunction* VMAdd = new VMFunction(*argumentTypes,
                                              (VMRawFunction) &vm_add);
    return *VMAdd;
  }

  VMObject* vm_sub(std::vector<VMObject*>& objects) {
    auto left = dynamic_cast<VMInt*>(objects[0]);
    auto right = dynamic_cast<VMInt*>(objects[1]);
    return new VMInt(left->value - right->value);
  }

  VMFunction& _getVMSub() {
    static auto argumentTypes = new std::vector<VMClass*> {
      getVMIntClass(), getVMIntClass()
    };
    static VMFunction* VMSub = new VMFunction(*argumentTypes,
                                              (VMRawFunction) &vm_sub);
    return *VMSub;
  }

  VMObject* vm_mul(std::vector<VMObject*>& objects) {
    auto left = dynamic_cast<VMInt*>(objects[0]);
    auto right = dynamic_cast<VMInt*>(objects[1]);
    return new VMInt(left->value * right->value);
  }

  VMFunction& _getVMMul() {
    static auto argumentTypes = new std::vector<VMClass*> {
      getVMIntClass(), getVMIntClass()
    };
    static VMFunction* VMMul = new VMFunction(*argumentTypes,
                                              (VMRawFunction) &vm_mul);
    return *VMMul;
  }

  VMObject* vm_div(std::vector<VMObject*>& objects) {
    auto left = dynamic_cast<VMInt*>(objects[0]);
    auto right = dynamic_cast<VMInt*>(objects[1]);
    return new VMInt(left->value / right->value);
  }

  VMFunction& _getVMDiv() {
    static auto argumentTypes = new std::vector<VMClass*> {
      getVMIntClass(), getVMIntClass()
    };
    static VMFunction* VMDiv = new VMFunction(*argumentTypes,
                                              (VMRawFunction) &vm_div);
    return *VMDiv;
  }

}
