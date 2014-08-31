#include "./builtins.hpp"
#include "./exceptions.hpp"

namespace VM {

  void vm_print(VMObject* object) {
    if (object->getType() != &VMStringClass) {
      throw VMException("cannot print non-string!");
    }
    VMClass* vmClass = object->getType();
    if (vmClass == &VMStringClass) {
      printf(((VMString*) object)->value.c_str());
    }
  }

}
