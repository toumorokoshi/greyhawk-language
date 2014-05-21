#include <string>
#include <iostream>

using std::string;

class A {
public:
  virtual string foo() const { return "foo"; }
};


class B : public A {
public:
  string foo() const { return "bar"; }
};

int main() {

  const B b();
  const A a();
  const B b_as_a();

  std::cout << a->foo() << std::endl;
  std::cout << b->foo() << std::endl;
  std::cout << b_as_a->foo() << std::endl;

}
