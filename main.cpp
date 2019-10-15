#include <iostream>
#include "stack.hpp"

struct A {
  int a;
  char b;
};

std::ostream& operator<<(std::ostream& out, A a) {
  out << a.a << ' ' << a.b;
  return out;
}

int main() {
  const int size = 5;
  const int N = 10;

  StaticStack<A, size> stack;

  for (int i = 0; i < N; ++i) {
    stack.push(A{1, 'a'});
  }

  for (int i = 0; i < N; ++i) {
    stack.pop();
  }

  return 0;
}

