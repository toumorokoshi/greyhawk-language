#include <gtest/gtest.h>
#include "../vm.hpp"

using namespace VM;


TEST(VM, helloWorld) {

  auto arguments = new std::vector<VMObject*>();
  arguments.push_back(new VMString("hello world"));
  auto statement = new VMCall("print", arguments);

  auto topScope = new VMScope();
  topScope.executeStatement(statement);
}
