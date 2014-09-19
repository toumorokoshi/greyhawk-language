#include "../class.hpp"
#include "../object.hpp"
#include "../string.hpp"
#include "../basic_types/bool.hpp"
#include "../exceptions.hpp"

#ifndef VM_BUILTINS_RANGE_HPP
#define VM_BUILTINS_RANGE_HPP

namespace VM {

  VMObject* vm_range(std::vector<VMObject*>& objects);
  VMFunction& _getVMRange();
  VMClass* getVMIntRangeClass();

  class VMIntRange : public VMObject {
  public:
    VMIntRange(int max) : _curVal(0), _max(max) {}

    virtual VMClass* getType() { return getVMIntRangeClass(); }

    VMBool* hasNext() { return new VMBool(_curVal < _max); }

    VMInt* next() {
      if (!hasNext()) {
        throw VMException("IntRange has no next element!");
      }
      return new VMInt(_curVal++);
    }

    VMString* toString() { return new VMString("IntRange(" + std::to_string(_max) + ")"); }

    VMObject* call(std::string methodName, std::vector<VMObject*>&) {

      if      (methodName == "hasNext") {  return hasNext(); }
      else if (methodName == "next") { return next(); }
      else if (methodName == "toString") { return toString(); }

      throw VMException("IntRange has no method " + methodName);

    }

  private:
    int _curVal;
    int _max;
  };

}

#endif
