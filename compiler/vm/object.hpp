#include "./class.hpp"
#include "./string.hpp"
#include "exceptions.hpp"
#include <functional>

#ifndef VM_OBJECT_HPP
#define VM_OBJECT_HPP

namespace VM {

  class VMObject {
  public:
    virtual VMClass* getType() = 0;
    VMObject* call(std::string, std::vector<VMObject*>&);
    virtual ~VMObject() {}
  };

  typedef std::vector<VMObject*> VMObjectList;

  class VMString : public VMObject {
  public:
    std::string value;
    virtual VMClass* getType() { return getVMStringClass(); };
    VMString(std::string _value) :
      value(_value) {}
  };

  class VMInt : public VMObject {
  public:
    int value;
    virtual VMClass* getType() { return getVMIntClass(); };
    VMInt(int _value): value(_value) {}
  };

  typedef VMObject* (*VMRawFunction)(VMObjectList&);
  typedef VMObject* (*VMRawMethod)(VMObject*, VMObjectList&);

  class VMFunction : public VMObject {
  public:
    virtual VMClass* getType() { return getVMFunctionClass(); };
    VMObject* call(VMObjectList& arguments);
    VMFunction(std::vector<VMClass*>& argumentTypes,
               VMRawFunction rawFunction) :
      _argumentTypes(argumentTypes),
      _rawFunction(rawFunction) {}
  private:
    std::vector<VMClass*>& _argumentTypes;
    VMRawFunction _rawFunction;
  };

  class VMMethod : public VMObject {
  public:
    virtual VMClass* getType() { return getVMMethodClass(); }
    VMObject* call(VMObject* self, VMObjectList& arguments);
    VMMethod(std::vector<VMClass*>& argumentTypes, VMRawMethod rawMethod);
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
