#include "./class.hpp"

#ifndef VM_OBJECT_HPP
#define VM_OBJECT_HPP

namespace VM {

  class VMObject {
  public:
    virtual VMClass* getType() = 0;
    virtual ~VMObject() {}
  };

  class VMString : public VMObject {
  public:
    std::string value;
    virtual VMClass* getType() { return &VMStringClass; };
    VMString(std::string _value) :
      value(_value) {}
  };

  class VMStructInstance : public VMObject {
  public:
    VMClass* classInstance;
    std::map<std::string, VMObject&> attributes;
    VMStructInstance(VMClass* _classInstance,
                     std::map<std::string, VMObject&> _attributes) :
      classInstance(_classInstance), attributes(_attributes) {}
  };
}

#endif
