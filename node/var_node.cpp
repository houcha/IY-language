#include "var_node.hpp"
#include "const_node.hpp"
#include <cstring>


MathNode* VarNode::DiffThis(const char* var, FILE* texfile) const {
  if (strcmp(name_.data(), var) == 0) {
    return new ConstNode<int>(1);
  }
  return new ConstNode<int>(0);
}

void VarNode::DumpDiffThis(const char* var, FILE* texfile) const {
  if (strcmp(name_.data(), var) == 0) {
    fprintf(texfile, "1");
  } else {
    fprintf(texfile, "0");
  }
}

uint8_t VarNode::GetPriority() const {
  return 0;
}

MathNode* VarNode::CopyThis() const {
  return new VarNode(*this);
}

uint32_t VarNode::GetThisComplexity() const {
  return 5;
}

const std::string VarNode::GetString() const {
  return name_;
}

const char* VarNode::GetColor() const {
  return "deeppink";
}



