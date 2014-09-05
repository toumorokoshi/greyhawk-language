#include <gtest/gtest.h>
#include <string>
#include "../vm.hpp"

using namespace VM;


TEST(VM, helloWorld) {
  auto arguments = new std::vector<VMObject*>();
  arguments->push_back(new VMString("hello world"));
  auto call = new VMCall("print", *arguments);
  try{
    call->run(getBuiltinScope());
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
