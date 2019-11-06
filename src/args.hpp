#ifndef ARGS_HPP
#define ARGS_HPP


// sizeof(arg) == sizeof(void*)
// 'toparse' is mark for python generator.
enum Arg : uint64_t {
  NoneArg,    // toparse
  Register,   // toparse
  Ftregister, // toparse
  Number,     // toparse
  Ftnumber,   // toparse
  Label,      // toparse
};

typedef uint8_t register_type;
typedef int8_t  ftregister_type;
typedef int64_t number_type;
typedef double  ftnumber_type;
typedef int32_t label_type;


/// Check if word is number.
template <typename String>
bool IsNumberArg(const String& word) {
  for (char letter : word) {
    if (!(('0' <= letter && letter <= '9') || letter == '-')) {
      return false;
    }
  }
  return true;
}

/// Check if word is float number.
template <typename String>
bool IsFtnumberArg(const String& word) {
  for (char letter : word) {
    if (!(('0' <= letter && letter <= '9') || letter == '.' || letter == '-')) {
      return false;
    }
  }
  return true;
}

/// Check if word is float register.
template <typename String>
bool IsFtregisterArg(const String& word) {
  return word[0] == 'x' && word[1] == 'm' && word[2] == 'm' &&
      (0 <= std::stoi(word.data() + 3) && std::stoi(word.data() + 3) <= 15);
}

/// Check if word is register.
template <typename String>
bool IsRegisterArg(const String& word) {
  return word[0] == 'r' && (0 <= std::stoi(word.data() + 1) &&
      std::stoi(word.data() + 1) <= 15);
}

/// Dummy.
template <typename String>
bool IsLabelArg(const String& word) {
  return true;
}


#endif // ARGS_HPP

