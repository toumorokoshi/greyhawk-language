#include <map>
#include "type.hpp"
#include "object.hpp"

#ifndef VM_CONTEXT_HPP
#define VM_CONTEXT_HPP

namespace VM {

  class GScope {
  public:
    std::map<std::string, GObject*> locals;

    GScope(GScope* parent) : _parentScope(parent) {}
    GScope() {}

    GType* getObjectType(std::string name) {
      if (types.find(name) != types.end()) {
        return types[name];
      }

      if (_parentScope != NULL) {
        return _parentScope->getObjectType(name);
      }

      return NULL;
    }

    void setType(std::string name, GType* type) {
      types[name] = type;
    }

  private:
    GScope* _parentScope;
    std::map<std::string, GType*> types;
  };

}
#endif
