#include <iostream>
#include "stack.hpp"

int main() {
  const int size = 5;
  const int N = 10;

  StaticStack<int, 5> stack;
  DynamicStack<int> stack1;

  for (int i = 0; i < N; ++i) {
    stack.push(i);
  }

  for (int i = 0; i < N; ++i) {
    stack.pop();
  }

  return 0;
}

