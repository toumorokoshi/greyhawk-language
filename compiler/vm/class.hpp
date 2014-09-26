#include <string>
#include <map>
#include <vector>
#include "exceptions.hpp"

#ifndef VM_CLASS_HPP
#define VM_CLASS_HPP

namespace VM {

  class VMMethod;
  class VMFunction;

  class VMClass {
  public:
    virtual bool matches(VMClass* otherClass) { return this == otherClass; }
    std::string name;
    VMFunction* constructor;
    std::map<std::string, VMMethod*> methods;
    VMClass(std::string _name) : name(_name) {}
    virtual ~VMClass() {}
  };

  class VMStruct : public VMClass {
  public:
    std::map<std::string, std::string>& attributes;
    VMStruct(std::string _name, std::map<std::string, std::string>& _attributes) :
      VMClass(_name), attributes(_attributes) {}
  };
}


#endif
