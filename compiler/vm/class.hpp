#include <string>
#include <map>

#ifndef VM_CLASS_HPP
#define VM_CLASS_HPP

namespace VM {

  class VMClass {
  public:
    std::map<std::string, std::string>& attributes;
    VMClass(std::map<std::string, std::string>& _attributes) :
      attributes(_attributes) {}
  };

}

#endif
