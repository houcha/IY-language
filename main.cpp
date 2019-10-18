#include <iostream>
#include "stack.hpp"

struct A {
  int a;
  char b;
};

template<>
A Poison<A>() {
  A a;
  a.a = 66666;
  a.b = 25;
  return a;
}

bool operator==(const A& a, const A& b) {
  return a.a == b.a && a.b == b.b;
}

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

