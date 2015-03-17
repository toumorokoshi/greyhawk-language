#include <map>
#include "ops.hpp"
#include "type.hpp"
#include "object.hpp"
#include "frame.hpp"

#ifndef VM_CONTEXT_HPP
#define VM_CONTEXT_HPP

namespace VM {

  struct GScopeInstance;

  // NOTE: resizing registers is dangerous!
  // do not allow this as a method until we can
  // figure out how to make this thread safe.
  /* class GScope {
  public:
    GFrame* frame;
    std::map<std::string, GOldFunction*> functionTable;
    std::map<std::string, GObject*> symbolTable;

    GScope(GScope* parent) : frame(parent->frame), _parent(parent) {}
    GScope(GScope* parent, GFrame* _frame) : frame(_frame), _parent(parent) {}
    GScope(GFrame* _frame) : frame(_frame) {}

    GObject* getObject(std::string name) {
      if (symbolTable.find(name) != symbolTable.end()) {
        return symbolTable[name];
      } else if (_parent != NULL) {
        return _parent->getObject(name);
      } else {
        return frame->getObject(name);
      }
    }

    GObject* addObject(std::string name, GType* type) {
      auto newObject = frame->allocateObject(type);
      symbolTable[name] = newObject;
      return newObject;
    }

    GOldFunction* getFunction(std::string name);

    void addFunction(std::string name, GOldFunction* function) {
      functionTable[name] = function;
    }

  private:
    GScope* _parent;
    }; */


  // we use a class instead of a struct
  // so we can encapsulate things for now,
  // until a good mechanism is decided.
  class GScope {
  public:
    // globals data
    GValue** globals;
    std::map<std::string, int> globalsTable;
    int globalsCount;

    // locals data
    std::map<std::string, int> localsTable;
    int localsCount;

    // get the index of an object
    // negative = global scope (GLOBAL_LOAD)
    // positive = local scope (LOAD)
    int getObject(std::string name) {
      if (localsTable.find(name) != localsTable.end()) {
        return localsTable[name];
      }

      if (globalsTable.find(name) != globalsTable.end()) {
        return - globalsTable[name];
      }

      return -1000;
    }

    GObject* addObject(std::string name, GType* type) {
      int index = allocateObject();
      localsTable[name] = index;
      return new GObject {
        .registerNum = index,
        .type = type
      };
    }

    int allocateObject() { return localsCount++; }
    GScopeInstance createInstance();
  };

  struct GScopeInstance {
    GScope* scope;
    GValue* values;

    GValue getValue(std::string);
  };

}
#endif
