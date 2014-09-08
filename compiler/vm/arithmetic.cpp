#include "./arithmetic.hpp"

namespace VM {

  VMObject* vm_add(std::vector<VMObject*>& objects) {
    auto left = dynamic_cast<VMInt*>(objects[0]);
    auto right = dynamic_cast<VMInt*>(objects[0]);
    return new VMInt(left->value + right->value);
  }

  VMObject* vm_sub(std::vector<VMObject*>& objects) {
    auto left = dynamic_cast<VMInt*>(objects[0]);
    auto right = dynamic_cast<VMInt*>(objects[0]);
    return new VMInt(left->value - right->value);
  }

  VMObject* vm_mul(std::vector<VMObject*>& objects) {
    auto left = dynamic_cast<VMInt*>(objects[0]);
    auto right = dynamic_cast<VMInt*>(objects[0]);
    return new VMInt(left->value * right->value);
  }

  VMObject* vm_div(std::vector<VMObject*>& objects) {
    auto left = dynamic_cast<VMInt*>(objects[0]);
    auto right = dynamic_cast<VMInt*>(objects[0]);
    return new VMInt(left->value * right->value);
  }

}
