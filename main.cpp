#include <iostream>
#include "stack.hpp"

int main() {
  const int stk_size = 2;

  StaticStack<int, stk_size> stk;

  DynamicStack<int> dstk;

  return 0;
}
