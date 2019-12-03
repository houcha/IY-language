def is_unary_operator(operator):
    return operator in {'-'}


def is_binary_operator(operator):
    return operator in {'+', '-', '*', '/', '^'}


def gen_create_node_cpp(operators, output_file):
    output_file.write(
"""\
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

"""
    )
    # Operators.
    for symbol, name in operators.items():
        output_file.write(
"""
  if (strncmp(begin, "{}", length) == 0) {{
    return new Operator{}Node;
  }} else\
""".format(symbol, name.capitalize())
    )
    # Constants.
    output_file.write(
"""
  if ((tmp = GetNumberType(begin, length)) != 0) {
    switch (tmp) {
      case 1: return new ConstNode<int>(atoi(begin));
      case 2: return new ConstNode<double>(atof(begin));
      default: assert("Unknown number type");
    }
  } else\
"""
    )
    # Variables.
    output_file.write(
"""
  if (IsCorrectVarName(begin, length)) {
    return new VarNode(begin, length);
  } else\
"""
    )
    output_file.write(
"""
  {
    assert("Incorrect lexeme");
  }
  return new EmptyNode;
}

"""
    )


def gen_operators_hpp(operators, output_file):
    output_file.write(
"""\
#ifndef OPERATORS_HPP
#define OPERATORS_HPP

#include "math_node.hpp"

"""
    )
    for symbol, name in operators.items():
        if is_unary_operator(symbol):
            output_file.write(
"""
MathNode* operator{}(MathNode& node);\
""".format(symbol)
            )
        if is_binary_operator(symbol): output_file.write(
"""
MathNode* operator{}(MathNode& lhs, MathNode& rhs);\
""".format(symbol)
            )
    output_file.write(
"""

#endif // OPERATORS_HPP

"""
    )


def gen_operators_cpp(operators, output_file):
    output_file.write(
"""\
#include "operators.hpp"
#include "operator_node.hpp"

"""
    )
    for symbol, name in operators.items():
        if is_unary_operator(symbol):
            output_file.write(
"""
MathNode* operator{}(MathNode& node) {{
  MathNode* operator_node = new Operator{}Node;
  operator_node->AddChild(&node);
  return operator_node;
}}
""".format(symbol, name.capitalize())
            )
        if is_binary_operator(symbol):
            output_file.write(
"""
MathNode* operator{}(MathNode& lhs, MathNode& rhs) {{
  MathNode* operator_node = new Operator{}Node;
  operator_node->AddChild(&lhs);
  operator_node->AddChild(&rhs);
  return operator_node;
}}
""".format(symbol, name.capitalize())
            )


def gen_operator_node_hpp(operators, output_file):
    output_file.write(
"""\
#ifndef OPERATOR_NODE_HPP
#define OPERATOR_NODE_HPP

#include "math_node.hpp"
#include <string>


/// Ancestor of any operator node.
class OperatorNode : public MathNode {
  protected:
    const char* GetColor() const override { return "gold"; }
};

"""
    )
    for symbol, name in operators.items():
        output_file.write(
"""
class Operator{}Node : public OperatorNode {{

  public:

    MathNode* Differentiate(const char* var) const override;

  protected:

    MathNode*         CopyThis()  const override {{ return new Operator{}Node; }}
    const std::string GetString() const override {{ return std::string("{}"); }}
}};

""".format(name.capitalize(), name.capitalize(), symbol)
        )
    output_file.write(
"""
#endif // OPERATOR_NODE_HPP

"""
    )


if __name__ == "__main__":
    operators = {
            "+": "add",
            "-": "sub",
            "*": "mul",
            "/": "div",
            "sin": "sin",
            "cos": "cos",
            "tan": "tan",
            "cot": "cot",
    }
    with open("create_node.cpp", 'w') as output_file:
        gen_create_node_cpp(operators, output_file)
    with open("operators.hpp", 'w') as output_file:
        gen_operators_hpp(operators, output_file)
    with open("operators.cpp", 'w') as output_file:
        gen_operators_cpp(operators, output_file)
    with open("operator_node.hpp", 'w') as output_file:
        gen_operator_node_hpp(operators, output_file)

