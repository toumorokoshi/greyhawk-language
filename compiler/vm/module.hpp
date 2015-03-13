#include "object.hpp"
#include "frame.hpp"
#include "ops.hpp"

#ifndef VM_MODULE_HPP
#define VM_MODULE_HPP

namespace VM {

    typedef struct GModule {
        std::map<std::string, GValue> globals;
        GFrame* frame;
        GInstruction* instructions;
      } GModule;

}

#endif
