#ifndef OPERATORS_HPP
#define OPERATORS_HPP

#include "math_node.hpp"


MathNode* operator+(MathNode& lhs, MathNode& rhs);
MathNode* operator-(MathNode& node);
MathNode* operator-(MathNode& lhs, MathNode& rhs);
MathNode* operator*(MathNode& lhs, MathNode& rhs);
MathNode* operator/(MathNode& lhs, MathNode& rhs);

#endif // OPERATORS_HPP

