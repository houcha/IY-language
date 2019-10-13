#include <iostream>
#include "stack.hpp"

int main() {
  const int size = 5;
  const int N = 10;

  StaticStack<int, size> stack;

  for (int i = 0; i < N; ++i) {
    stack.push(i);
    std::cout << "i: " << i << " top(): " << stack.top() << '\n';
  }

  for (int i = 0; i < N; ++i) {
    stack.pop();
    std::cout << "i: " << i << " top(): " << stack.top() << '\n';
  }

  return 0;
}

