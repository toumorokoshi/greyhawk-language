/*
   Frames contain the current context in which the
   application runs.
 */

#include "object.hpp"

#ifndef VM_FRAME_HPP
#define VM_FRAME_HPP

namespace VM {

  struct GFrame {
    GObject** registers;
  };
}

#endif
