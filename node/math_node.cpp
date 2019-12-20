#include "math_node.hpp"


uint32_t MathNode::GetComplexity() const {
  return info_.complexity_;
}

MathNode* MathNode::GetCopy() const {
  MathNode* copy = CopyThis();
  if (GetLeft() != nullptr) { copy->AddChild(GetLeft()->GetCopy()); }
  if (GetRight() != nullptr) { copy->AddChild(GetRight()->GetCopy()); }
  return copy;
}

MathNode* MathNode::Simplify() {
  if (GetLeft() != nullptr) { ReplaceLeft(GetLeft()->Simplify()); }
  if (GetRight() != nullptr) { ReplaceRight(GetRight()->Simplify()); }
  return SimplifyThis();
}

MathNode* MathNode::SimplifyThis() {
  return this;
}

MathNode* MathNode::GetLeft() const {
  return (MathNode*)(Node::GetLeft());
}

MathNode* MathNode::GetRight() const {
  return (MathNode*)(Node::GetRight());
}

MathNode* MathNode::GetParent() const {
  return (MathNode*)(Node::GetParent());
}

const char* MathNode::GetColor() const {
  return "cyan";
}

void MathNode::Update() {
  info_.complexity_ = GetThisComplexity();
  if (GetLeft() != nullptr) {
    info_.complexity_ += GetLeft()->GetComplexity();
  }
  if (GetRight() != nullptr) {
    info_.complexity_ += GetRight()->GetComplexity();
  }
}

