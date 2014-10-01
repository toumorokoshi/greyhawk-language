#include <map>
#include <vector>
#include "./class.hpp"
#include "./object.hpp"
#include "./exceptions.hpp"

#ifndef VM_CONTEXT_HPP
#define VM_CONTEXT_HPP

namespace VM {

  class VMScope {
  public:
    std::map<std::string, VMObject*> locals;
    std::map<std::string, VMClass*> localTypes;

    VMScope(VMScope* parent) : _parentScope(parent) {}
    VMScope() {}
    VMObject* invokeMethod(std::string methodName, std::vector<VMObject*>& args);

    VMObject* getObject(std::string name) {
      if (locals.find(name) != locals.end()) {
        return locals[name];
      }

      if (_parentScope != NULL) {
        return _parentScope->getObject(name);
      }

      throw VMException(name + " is not defined!");

    }

  private:
    VMScope* _parentScope;
  };

}
#endif
