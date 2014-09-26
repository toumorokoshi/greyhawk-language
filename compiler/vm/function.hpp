#include "block.hpp"
#include "object.hpp"


#ifndef VM_FUNCTION_HPP
#define VM_FUNCTION_HPP

namespace VM {

  typedef VMObject* (*VMRawFunction)(VMObjectList&);

  VMClass* getVMFunctionClass();

  class VMFunction : public VMObject {
  public:
    virtual ~VMFunction() {}
    virtual VMClass* getType() { return getVMFunctionClass(); };
    virtual VMObject* call(VMScope& scope, VMObjectList& arguments) = 0;
  };

  class VMRawFunctionWrapper : public VMFunction {
  public:
    virtual VMObject* call(VMScope& scope, VMObjectList& arguments);
    VMRawFunctionWrapper(std::vector<VMClass*>& argumentTypes,
                         VMRawFunction rawFunction) :
      _argumentTypes(argumentTypes),
      _rawFunction(rawFunction) {}
  private:
    std::vector<VMClass*>& _argumentTypes;
    VMRawFunction _rawFunction;
  };

  typedef std::pair<std::string, std::string> VMArgumentDefinition;
  typedef std::vector<VMArgumentDefinition*> VMArgumentList;

  class VMFunctionDeclaration : public VMStatement, public VMFunction {
  public:
    std::string functionName;
    VMBlock* body;
    VMArgumentList& arguments;
    virtual VMObject* execute(VMScope& scope);
    virtual VMObject* call(VMScope& scope, VMObjectList& arguments);
    VMFunctionDeclaration(std::string _functionName,
                          VMArgumentList& _arguments,
                          VMBlock* _body):
      functionName(_functionName), arguments(_arguments), body(_body) {}
  };
}

#endif
