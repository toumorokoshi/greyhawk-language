/*
   Frames contain the current context in which the
   application runs.
 */

#include "object.hpp"
#include <map>

#ifndef VM_FRAME_HPP
#define VM_FRAME_HPP

namespace VM {

  // TODO: figure out a way to re-use old registers
  class GFrame {
  public:
    int registerCount;
    std::map<std::string, GObject*> symbolTable;

    GObject* getObject(std::string name) {
      return symbolTable.find(name) != symbolTable.end() ? symbolTable[name] : NULL;
    }

    void addObject(std::string name, GType* type) {
      symbolTable[name] = allocateObject(type);
    }

    GObject* allocateObject (GType* type) {
      return new GObject { type, registerCount++ };
    }
  };
}

#endif
