#include <map>
#include <vector>
#include "./class.hpp"
#include "./object.hpp"

#ifndef VM_CONTEXT_HPP
#define VM_CONTEXT_HPP

namespace VM {

  class VMScope {
  public:
    std::map<std::string, VMClass*> classes;
    std::map<std::string, VMObject*> locals;
    VMScope(VMScope* parent) : _parentScope(parent) {}
  private:
    VMScope* _parentScope;
    VMObject* invokeMethod(std::string methodName, std::vector<VMObject*> args);
  };

}
#endif
