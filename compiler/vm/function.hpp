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
    virtual VMClass* getReturnType() = 0;
    virtual void validateTypes(std::vector<VMClass*>& types) = 0;
    virtual VMObject* call(VMScope& scope, VMObjectList& arguments) = 0;
  };

  class VMRawFunctionWrapper : public VMFunction {
  public:
    virtual VMObject* call(VMScope& scope, VMObjectList& arguments);
    virtual void validateTypes(std::vector<VMClass*>&) {}
    virtual VMClass* getReturnType() { return _returnType; }
    VMRawFunctionWrapper(VMClass* returnType,
                         std::vector<VMClass*>& argumentTypes,
                         VMRawFunction rawFunction) :
      _returnType(returnType),
      _argumentTypes(argumentTypes),
      _rawFunction(rawFunction) {}
  private:
    VMClass* _returnType;
    std::vector<VMClass*>& _argumentTypes;
    VMRawFunction _rawFunction;
  };

  typedef std::pair<std::string, std::string> VMArgumentDefinition;
  typedef std::vector<VMArgumentDefinition*> VMArgumentList;

  class VMFunctionDeclaration : public VMStatement, public VMFunction {
  public:
    std::string functionName;
    VMClass* returnType;
    VMArgumentList& arguments;
    VMBlock* body;
    virtual VMObject* execute(VMScope& scope);
    virtual void validateTypes(std::vector<VMClass*>&) {}
    virtual VMClass* getReturnType() { return returnType; }
    virtual VMObject* call(VMScope& scope, VMObjectList& arguments);
    VMFunctionDeclaration(std::string _functionName,
                          VMClass* _returnType,
                          VMArgumentList& _arguments,
                          VMBlock* _body):
      functionName(_functionName), returnType(_returnType),
      arguments(_arguments), body(_body) {}
  };
}

#endif
