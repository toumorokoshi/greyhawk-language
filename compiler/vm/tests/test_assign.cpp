#include <gtest/gtest.h>
#include <string>
#include "../vm.hpp"

using namespace VM;

TEST(VM, testAssign) {
  auto scope = new VMScope(getBuiltinScope());
  auto assign = new VMDeclare("foo",
                              new VMConstant(new VMString("test")));
  assign->execute(*scope);
  auto foo = scope->getObject("foo");
  EXPECT_EQ(foo->getType(), getVMStringClass());
  EXPECT_EQ(dynamic_cast<VMString*>(foo)->value, "test");
}
