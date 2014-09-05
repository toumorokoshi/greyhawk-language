#include <functional>
#include <vector>
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
    virtual VMClass* getType() { return getVMStringClass(); };
    VMString(std::string _value) :
      value(_value) {}
  };

  typedef VMObject* (*VMRawMethod)(std::vector<VMObject*>&);

  class VMMethod : public VMObject {
  public:
    virtual VMClass* getType() { return getVMMethodClass(); };
    VMObject* call(std::vector<VMObject*>& arguments);
    VMMethod(std::vector<VMClass*>& argumentTypes,
             VMRawMethod rawMethod) :
      _argumentTypes(argumentTypes),
      _rawMethod(rawMethod) {}
  private:
    std::vector<VMClass*>& _argumentTypes;
    VMRawMethod _rawMethod;
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
