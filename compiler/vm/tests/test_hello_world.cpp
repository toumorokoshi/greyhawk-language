#include <gtest/gtest.h>
#include <string>
#include "../vm.hpp"

using namespace VM;


TEST(VM, helloWorld) {
  auto arguments = new std::vector<VMExpression*>();
  auto argument = new VMConstant(new VMString("hello world"));
  arguments->push_back(argument);
  auto call = new VMCall("print", *arguments);
  try{
    call->execute(getBuiltinScope());
  } catch (VMException& e) {
    printf(e.message.c_str());
    throw e;
  }
}

TEST(VM, printBuiltin) {

  auto arguments = new std::vector<VMObject*>();
  arguments->push_back(new VMString("hello world"));
  vm_print(*arguments);
}

TEST(VM, helloWorldScope) {
  auto scope = new VMScope(getBuiltinScope());
  scope->getObject("print");
}
