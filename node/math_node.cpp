#include "math_node.hpp"
#include "string_out.hpp"


const size_t MAX_INFO_STR_LEN = 100;


MathNode* MathNode::Differentiate(const char* var, FILE* texfile) const {
  if (texfile != nullptr) {
    fprintf(texfile, "$$\\left(");
    WriteToTex(texfile);
    fprintf(texfile, "\\right)_x^{'} = ");
    DumpDiffThis(var, texfile);
    fprintf(texfile, "$$\n");
  }
  DiffThis(var, texfile);
}

uint32_t MathNode::GetComplexity() const {
  return info_.complexity_;
}

MathNode* MathNode::GetCopy() const {
  MathNode* copy = CopyThis();
  if (GetLeft() != nullptr) { copy->AddChild(GetLeft()->GetCopy()); }
  if (GetRight() != nullptr) { copy->AddChild(GetRight()->GetCopy()); }
  return copy;
}

MathNode* MathNode::Simplify(FILE* texfile) {
  if (GetLeft() != nullptr) { ReplaceLeft(GetLeft()->Simplify(texfile)); }
  if (GetRight() != nullptr) { ReplaceRight(GetRight()->Simplify(texfile)); }
  return SimplifyThis(texfile);
}

MathNode* MathNode::SimplifyThis(FILE* texfile) {
  return this;
}

void MathNode::WriteThisToGraphviz(FILE* graphfile) const {
    fprintf(graphfile, "node%p [label=\"%.*s\" style=filled fillcolor=%s]\n",
        (void*)this,
        UNFOLD(GetString()),
        GetColor());
}

void MathNode::WriteToGraphviz(FILE* graphfile) const {
  if (GetParent() != nullptr) {
    GetParent()->WriteThisToGraphviz(graphfile);
  }
  WriteThisToGraphviz(graphfile);
  fprintf(graphfile, "node%p -> node%p\n", (void*)GetParent(), (void*)this);
}

void MathNode::WriteToTex(FILE* texfile) const {
  if (GetRight() == nullptr) { // If unary operator.
    fprintf(texfile, " ");
    WriteThisToTex(texfile);
    if (GetLeft() != nullptr) {
      if (GetLeft()->GetPriority() >= GetPriority()) {
        fprintf(texfile, "(");
      }
      GetLeft()->WriteToTex(texfile);
      if (GetLeft()->GetPriority() >= GetPriority()) {
        fprintf(texfile, ")");
      }
    }
  } else {
    if (GetLeft() != nullptr) {
      if (GetLeft()->GetPriority() >= GetPriority()) {
        fprintf(texfile, "(");
      }
      GetLeft()->WriteToTex(texfile);
      if (GetLeft()->GetPriority() >= GetPriority()) {
        fprintf(texfile, ")");
      }
    }
    fprintf(texfile, " ");
    WriteThisToTex(texfile);
    fprintf(texfile, " ");
    if (GetRight() != nullptr) {
      if (GetPriority() <= GetRight()->GetPriority()) {
        fprintf(texfile, "(");
      }
      GetRight()->WriteToTex(texfile);
      if (GetPriority() <= GetRight()->GetPriority()) {
        fprintf(texfile, ")");
      }
    }
  }
}

void MathNode::WriteThisToTex(FILE* texfile) const {
  fprintf(texfile, "%.*s", UNFOLD(GetString()));
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

