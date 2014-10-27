#include <map>
#include "type.hpp"
#include "object.hpp"
#include "frame.hpp"

#ifndef VM_CONTEXT_HPP
#define VM_CONTEXT_HPP

namespace VM {

  // NOTE: resizing registers is dangerous!
  // do not allow this as a method until we can
  // figure out how to make this thread safe.
  class GScope {
  public:
    GFrame* frame;

    GScope(GFrame* _frame, GScope* parent) : frame(_frame), _parent(parent) {}

    GScope(GFrame* _frame) : frame(_frame) {}

    GObject* getObject(std::string name) {
      if (_symbolTable.find(name) != _symbolTable.end()) {
        return _symbolTable[name];
      } else if (_parent != NULL) {
        return _parent->getObject(name);
      } else {
        return frame->getObject(name);
      }
    }

    GObject* addObject(std::string name, GType* type) {
      auto newObject = frame->allocateObject(type);
      _symbolTable[name] = newObject;
      return newObject;
    }

  private:
    std::map<std::string, GObject*> _symbolTable;
    GScope* _parent;
  };

}
#endif
