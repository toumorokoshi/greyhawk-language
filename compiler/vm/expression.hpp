#include "./object.hpp"
#include "./scope.hpp"

namespace VM {

  class VMExpression {
  public:
    virtual VMObject* run(VMScope& scope);
    virtual ~VMExpression();
  };

  class VMCall {
  public:
    std::string& methodName;
    std::vector<VMObject*>& arguments;

    virtual VMObject* run(VMScope& scope) {
      scope.invokeMethod(methodName, arguments);
    };

    VMCall(std::string& _methodName,
           std::vector<VMObject*>& _arguments) :
      methodName(_methodName), arguments(_arguments) {}
  };
}
