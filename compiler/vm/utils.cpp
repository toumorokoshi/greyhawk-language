#include "utils.hpp"

namespace VM {

  bool evaluate_vmbool(VMObject* b) {
    return dynamic_cast<VMBool*>(b)->value;
  }
}
