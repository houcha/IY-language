#include "var_node.hpp"
#include "const_node.hpp"
#include <cstring>


MathNode* VarNode::Differentiate(const char* var) const {
  if (strcmp(name_.data(), var) == 0) {
    return new ConstNode<int>(1);
  }
  return new ConstNode<int>(0);
}

MathNode* VarNode::CopyThis() const {
  return new VarNode(*this);
}

const std::string VarNode::GetString() const {
  return name_;
}

const char* VarNode::GetColor() const {
  return "deeppink";
}



