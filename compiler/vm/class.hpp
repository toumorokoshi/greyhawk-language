#include "function.hpp"
#include "object.hpp"
#include "ops.hpp"
#include <map>

#ifndef VM_CLASS_HPP
#define VM_CLASS_HPP

namespace VM {

  typedef struct GClass {
    std::map<std::string, GObject> attributes;
    std::map<std::string, GFunction*> methods;
  } GClass;

  void runMethod(GClass* object, std::string methodName);
}

#endif
