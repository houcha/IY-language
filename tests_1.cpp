#include <iostream>
#include "stack.hpp"

using namespace std;

class MyClass {
  int *array;

public:
  MyClass() {
    array = new int[10];
    cout << "MyClass c-tor" << endl;
  }

  ~MyClass() {
    cout << "~MyClass" << endl;
    delete array;
    array = nullptr;
  }

  MyClass(MyClass&& move_ctor) {
    cout << "MyClass move" << endl;
    array = move_ctor.array;
    move_ctor.array = nullptr;
  }

  MyClass(const MyClass& copy_ctor) {
    cout << "MyClass copy" << endl;
    array = new int[10];
  }

  MyClass& operator=(const MyClass&) {
    cout << "MyClass operator= copy" << endl;
    array = new int[10];
    return *this;
  }

  MyClass& operator=(MyClass&& rhs) {
    cout << "MyClass operator= move" << endl;
    array = rhs.array;
    rhs.array = nullptr;
    return *this;
  }

  friend std::ostream& operator<<(std::ostream& out, const MyClass& mc);
};

std::ostream& operator<<(std::ostream& out, const MyClass& mc) {
  //OPEN_SCOPE()
  //DUMP_CONTAINER_STATE(out, &mc, mc.array)
  out << "MyClass: reference to array is nullptr: " << (mc.array == nullptr) << endl;
  //CLOSE_SCOPE()
  return out;
}


int main() {
  DynamicStack<MyClass> st;
  cout << "Now pushing" << endl;
  MyClass my;
  st.push(my);
  st.push(my);
  st.push(my);
  st.push(my);
  st.push(my);
  cout << "Exiting" << endl;
  return 0;
}
