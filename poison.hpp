#ifndef POISON_H
#define POISON_H

#include <cstdlib>

template <typename T>
T Poison();

/// Return true if value is Poison.
template <typename T>
bool IsPoison(const T& value) {
  return value == Poison<T>();
}

/// Assign value with poison.
template <typename T>
void SetPoison(T& value) {
  value = Poison<T>();
}

/* Possible (but not workable) implementations:

template <typename T>
bool IsPoison(const T& value) {

  const char* raw = (const char*)&value;

  for (int i = 0; i < sizeof(T); ++i) {
    if (raw[i] != (char)(sizeof(T) * 666)) {
      return false;
    }
  }

  return true;
}


template <typename T>
void SetPoison(T& value) {

  char* raw = (char*)&value;

  for (int i = 0; i < sizeof(T); ++i) {
    raw[i] = (char)(sizeof(T) * 666);
  }
}

*/

#endif // POISON_H

