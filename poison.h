#ifndef POISON_H
#define POISON_H

#include <string>
#include <cstdlib>


/// Return true if value is Poison.
template <typename T>
bool IsPoison(const T& value);

/// Return Poison for T type
template <typename T>
T Poison();

/// Compare two elements for bytes equality.
template <typename T>
bool AreBytesEqual(const T& first, const T& second) {

  char*  first_ptr = (char*)&first;
  char* second_ptr = (char*)&second;
  bool are_equal = true;

  for (int i = 0; i < sizeof(T); ++i) {
    if (first_ptr[i] != second_ptr[i]) {
      are_equal = false;
      break;
    }
  }

  return are_equal;
}

template <typename T>
bool IsPoison(const T& value) {
  return (AreBytesEqual(value, Poison<T>()));
}

template <typename T>
T Poison() {

  std::srand(sizeof(T));
  char* raw = (char*)malloc(sizeof(T));

  for (int i = 0; i < sizeof(T); ++i) {
    raw[i] = std::rand();
  }

  T copy = reinterpret_cast<T&>(*raw);
  free(raw);

  return copy;
}


#endif // POISON_H

