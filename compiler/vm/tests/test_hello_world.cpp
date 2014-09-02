#include <gtest/gtest.h>
#include "../vm.hpp"

using namespace VM;


TEST(VM, helloWorld) {

  auto arguments = new std::vector<VMObject*>();
  arguments->push_back(new VMString("hello world"));
  auto topScope = new VMScope();
  auto statement = new VMCall("print", arguments);
  statement.run(topScope);
}
