#include "class.hpp"
#include "exceptions.hpp"
#include <functional>

#ifndef VM_OBJECT_HPP
#define VM_OBJECT_HPP

namespace VM {

  class VMObject {
  public:
    virtual VMClass* getType() = 0;
    virtual VMObject* call(std::string, std::vector<VMObject*>&);
    virtual ~VMObject() {}
  };

  typedef std::vector<VMObject*> VMObjectList;

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
