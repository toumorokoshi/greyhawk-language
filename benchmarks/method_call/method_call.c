#include <stdio.h>


void doMath(int a, int b) {
    printf("%d", a + b);
}

int main() {
  for (int i = 0; i < 10000; i++) {
    doMath(1, 2);
  }
}
