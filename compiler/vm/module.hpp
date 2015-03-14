#include "object.hpp"
#include "frame.hpp"
#include "ops.hpp"

#ifndef VM_MODULE_HPP
#define VM_MODULE_HPP

namespace VM {

  typedef struct GModule {
    // this houses the global values.
    // functionPointers, globals, classes, etc
    std::map<std::string, GValue> globals;
    std::map<std::string, GObject*> symbolTable;
    int registerCount; // the total # of registers needed to execute the module.
    GInstruction* instructions; // the instructions to actually execute

    GObject* getObject(std::string name) {
      return symbolTable.find(name) != symbolTable.end() ? symbolTable[name] : NULL;
    }

    void addObject(std::string name, GType* type) {
      symbolTable[name] = allocateObject(type);
    }

    GObject* allocateObject (GType* type) {
      return new GObject { type, registerCount++ };
    }
  } GModule;

}

#endif
