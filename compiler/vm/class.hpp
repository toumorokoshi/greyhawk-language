#include <string>
#include <map>

#ifndef VM_CLASS_HPP
#define VM_CLASS_HPP

namespace VM {

  class VMClass {
  public:
    virtual ~VMClass() {}
  };

  class VMStruct : public VMClass {
  public:
    std::map<std::string, std::string>& attributes;
    VMStruct(std::map<std::string, std::string>& _attributes) :
      attributes(_attributes) {}
  };

  VMClass* getVMStringClass();
  VMClass* getVMMethodClass();
  VMClass* getVMIntClass();
}

#endif
