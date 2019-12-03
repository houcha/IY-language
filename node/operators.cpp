#include "operators.hpp"
#include "operator_node.hpp"


MathNode* operator+(MathNode& lhs, MathNode& rhs) {
  MathNode* operator_node = new OperatorAddNode;
  operator_node->AddChild(&lhs);
  operator_node->AddChild(&rhs);
  return operator_node;
}

MathNode* operator*(MathNode& lhs, MathNode& rhs) {
  MathNode* operator_node = new OperatorMulNode;
  operator_node->AddChild(&lhs);
  operator_node->AddChild(&rhs);
  return operator_node;
}

MathNode* operator-(MathNode& node) {
  MathNode* operator_node = new OperatorSubNode;
  operator_node->AddChild(&node);
  return operator_node;
}

MathNode* operator-(MathNode& lhs, MathNode& rhs) {
  MathNode* operator_node = new OperatorSubNode;
  operator_node->AddChild(&lhs);
  operator_node->AddChild(&rhs);
  return operator_node;
}

MathNode* operator/(MathNode& lhs, MathNode& rhs) {
  MathNode* operator_node = new OperatorDivNode;
  operator_node->AddChild(&lhs);
  operator_node->AddChild(&rhs);
  return operator_node;
}
