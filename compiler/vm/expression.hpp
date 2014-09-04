#include "./object.hpp"
#include "./scope.hpp"

namespace VM {

  class VMExpression {
  public:
    virtual VMObject* run(VMScope& scope) = 0;
    virtual ~VMExpression() {}
  };

  class VMCall : VMExpression {
  public:
    std::string methodName;
    std::vector<VMObject*>& arguments;

    virtual VMObject* run(VMScope& scope) {
      return scope.invokeMethod(methodName, arguments);
    }

    VMCall(std::string _methodName,
           std::vector<VMObject*>& _arguments) :
      methodName(_methodName), arguments(_arguments) {}
  };
}
