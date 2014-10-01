#include "../class.hpp"
#include "../expression.hpp"
#include "../object.hpp"

#ifndef VM_STRING_HPP
#define VM_STRING_HPP

namespace VM {

  VMClass* getVMStringClass();

  class VMString : public VMObject, public VMExpression {
  public:
    std::string value;

    virtual VMClass* getType() { return getVMStringClass(); };
    virtual VMObject* evaluate(VMScope&) { return this; }

    VMString(std::string _value) :
      value(_value) {}
  };

}

#endif
