#include <map>
#include <vector>
#include "../vm/environment.hpp"
#include "../vm/function.hpp"

#ifndef VM_SCOPE_HPP
#define VM_SCOPE_HPP

#ifdef DEBUG
  #define debug(s) std::cerr << s << std::endl;
#else
  #define debug(s);
#endif

namespace parser {
  class PFunctionDeclaration;
}

namespace codegen {

  class GScope {
  public:
    VM::GEnvironment* environment;
    GScope* parentScope;

    std::map<std::string, int> typeIndexByName;
    std::map<std::string, VM::GIndex*> localsByName;
    std::map<std::string, int> functionIndexByName;
    // I think these have to be the last attributes referenced.
    // if not, this causes weird compile errors in clang.
    // std::vector<GFunction*> functions;
    std::vector<parser::PFunctionDeclaration*> functionDeclarations;

    VM::GIndex*    addObject(std::string name, VM::GType* type);
    VM::GIndex*    allocateObject(VM::GType* type);
    VM::GIndex*    getObject(std::string name);

    VM::GIndex*    addClass(std::string name, VM::GType* type);
    VM::GType*     getClass(std::string name);

    VM::GIndex*    addFunction(std::string name, VM::GFunction* func,
                               parser::PFunctionDeclaration* declaration);
    VM::GFunction* getFunction(std::string name);

    GScope*        createChild(bool);
    void           finalize();
  };

  VM::GEnvironment* createEnvironmentFromScope(GScope*);
}

#endif
