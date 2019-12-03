#include "math_node.hpp"
#include "string_out.hpp"


const size_t MAX_INFO_STR_LEN = 100;


MathNode* MathNode::GetCopy() const {
  MathNode* copy = CopyThis();
  if (left_ != nullptr) { copy->left_ = GetLeft()->GetCopy(); }
  if (right_ != nullptr) { copy->right_ = GetLeft()->GetCopy(); }
  return copy;
}

MathNode* MathNode::GetLeft() const {
  return (MathNode*)(left_);
}

MathNode* MathNode::GetRight() const {
  return (MathNode*)(right_);
}

MathNode* MathNode::GetParent() const {
  return (MathNode*)(parent_);
}

const std::string MathNode::GetGraphvizInfo() const {
  std::string info(MAX_INFO_STR_LEN, '0');
  snprintf(info.data(), MAX_INFO_STR_LEN,
      "[ "
      "label=\"%.*s\" "
      "style=filled fillcolor=%s "
      "]",
      UNFOLD(GetString()),
      GetColor());
  return info;
}

const char* MathNode::GetColor() const {
  return "cyan";
}

