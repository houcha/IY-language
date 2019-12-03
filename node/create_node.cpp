#include "math_node.hpp"
#include "const_node.hpp"
#include "var_node.hpp"
#include "operator_node.hpp"
#include <cstring>
#include <cassert>


inline bool IsDigit(char ch) {
  return '0' <= ch && ch <= '9';
}

inline bool IsLetter(char ch) {
  return 'a' <= ch && ch <= 'z';
}

/// Return 0 if string is not number, 1 if it is int and 2 if it is double.
int GetNumberType(const char* str, size_t len) {
  int number_type = 1;
  bool has_dot = false;

  size_t i = 0;
  if (str[0] == '-') { ++i; }

  for ( ; i < len; ++i) {
    if (!(IsDigit(str[i]) || (str[i] == '.' && !has_dot))) {
      return 0;
    }
    if (str[i] == '.') {
      number_type = 2;
      has_dot = true;
    }
  }
  return number_type;
}

bool IsCorrectVarName(const char* str, size_t len) {

  if (IsDigit(str[0])) { return false; }

  for (size_t i = 0; i < len; ++i) {
    if (!(IsLetter(str[i]) || IsDigit(str[i]))) {
      return false;
    }
  }
  return true;
}

MathNode* CreateNode(const char* begin, const char* end) {
  // Skip whitespace characters.
  while (*begin == ' ' && begin < end) { ++begin; }
  while (*end   == ' ' && begin < end) { --end; }
  // Return nullptr on whitespace string.
  if (*begin == ' ' && *end == ' ') { return nullptr; }

  size_t length = end - begin + 1;
  int tmp = 0; // Temporary variable.


  if (strncmp(begin, "cos", length) == 0) {
    return new OperatorCosNode;
  } else
  if (strncmp(begin, "tan", length) == 0) {
    return new OperatorTanNode;
  } else
  if (strncmp(begin, "+", length) == 0) {
    return new OperatorAddNode;
  } else
  if (strncmp(begin, "*", length) == 0) {
    return new OperatorMulNode;
  } else
  if (strncmp(begin, "-", length) == 0) {
    return new OperatorSubNode;
  } else
  if (strncmp(begin, "sin", length) == 0) {
    return new OperatorSinNode;
  } else
  if (strncmp(begin, "/", length) == 0) {
    return new OperatorDivNode;
  } else
  if (strncmp(begin, "cot", length) == 0) {
    return new OperatorCotNode;
  } else
  if ((tmp = GetNumberType(begin, length)) != 0) {
    switch (tmp) {
      case 1: return new ConstNode<int>(atoi(begin));
      case 2: return new ConstNode<double>(atof(begin));
      default: assert("Unknown number type");
    }
  } else
  if (IsCorrectVarName(begin, length)) {
    return new VarNode(begin, length);
  } else
  {
    assert("Incorrect lexeme");
  }
  return new EmptyNode;
}

