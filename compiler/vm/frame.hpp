/*
   Frames contain the current context in which the
   application runs.
 */

#include "object.hpp"

#ifndef VM_FRAME_HPP
#define VM_FRAME_HPP

namespace VM {

  class GFrame {
  public:
    // the total # of
    int registerCount;
    // if one 'deallocates', you can re-assign existing registers.
    // this is the current available registers
    int currentRegister;

  };
}

#endif
