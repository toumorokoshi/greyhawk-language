#include "range.hpp"

namespace VM {

  /** IntRange **/
  VMObject* intRangeConstructor(VMObjectList& args) {
    auto max = dynamic_cast<VMInt*>(*args.begin());
    return new VMIntRange(max->value);
  }

  VMClass* getVMIntRangeClass() {
    static auto IntRange = new VMClass("IntRange");
    static bool _initialized = false;
    if (!_initialized) {
      auto constructorArguments = new std::vector<VMClass*> {
        getVMIntClass()
      };
      auto constructor = new VMFunction(*constructorArguments,
                                        (VMRawFunction) &intRangeConstructor);

      IntRange->constructor = constructor;
      _initialized = true;
    }

    return IntRange;
  }


}
