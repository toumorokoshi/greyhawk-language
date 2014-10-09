#include "type.hpp"

#ifndef VM2_VALUE_HPP
#define VM2_VALUE_HPP

namespace VM {

  typedef struct {
    GType* type;
    void* value;
  } GValue;
}

#endif
