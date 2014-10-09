#include <string>

#ifndef VM2_TYPE_HPP
#define VM2_TYPE_HPP

namespace VM {

  enum BASICTYPES {
    CLASS,
    INT32,
    NONE
  };

  typedef struct {
    BASICTYPES classifier;
    std::string name;
  } GType;
}

#endif
