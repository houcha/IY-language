#ifndef OPERATORS_HPP
#define OPERATORS_HPP

#include "math_node.hpp"


MathNode* operator+(Node& lhs, Node& rhs);
MathNode* operator-(Node& node);
MathNode* operator-(Node& lhs, Node& rhs);
MathNode* operator*(Node& lhs, Node& rhs);
MathNode* operator/(Node& lhs, Node& rhs);

#endif // OPERATORS_HPP

