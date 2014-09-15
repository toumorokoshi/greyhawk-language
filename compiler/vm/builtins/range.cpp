#include "range.hpp"

namespace VM {

  /** range(Int) **/
  VMObject* vm_range(std::vector<VMObject*>& args) {
    auto upperBound = dynamic_cast<VMString*>(*args.begin());
    return NULL;
  }

  VMFunction& _getVMRange() {
    auto argumentTypes = new std::vector<VMClass*>();
    argumentTypes->push_back(getVMIntClass());

    return *new VMFunction(*argumentTypes, (VMRawFunction) &vm_range);
  }

  /** IntRange **/


  VMClass* getVMIntRangeClass() {
    static auto IntRange = new VMClass("IntRange");
    static bool _initialized = false;
    if (!_initialized) {
      // TODO: add next() and hasNext() methods
      // IntRange.methods
    }
  }


}
