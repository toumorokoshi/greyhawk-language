#include "environment.hpp"
#include "builtins.hpp"

#ifndef VM_ROOTENVIRONMENT_HPP
#define VM_ROOTENVIRONMENT_HPP
namespace VM {
  GEnvironment& getBaseEnvironment();
  GEnvironmentInstance& getBaseEnvironmentInstance();
}
#endif
