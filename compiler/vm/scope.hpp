#include <map>
#include "type.hpp"
#include "object.hpp"

#ifndef VM_CONTEXT_HPP
#define VM_CONTEXT_HPP

namespace VM {

  // NOTE: resizing registers is dangerous!
  // do not allow this as a method until we can
  // figure out how to make this thread safe.
  class GScope {
  public:
    int registerCount;
    int subRegisterCount;

    GScope(GScope* parent) : _parentScope(parent) {}
    GScope() {}

    bool hasObject(std::string name) {
      return symbolTable.find(name) != symbolTable.end();
    }

    GObject* getObject(std::string name) {
      return symbolTable[name];
    }

    void addObject(std::string name, GType* type) {
      symbolTable[name] = new GObject { type, registerCount++ };
    }

  private:
    std::map<std::string, GObject*> symbolTable;
    GScope* _parentScope;
  };

}
#endif
