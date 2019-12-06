#include "operator_node.hpp"
#include "const_node.hpp"
#include "operators.hpp"
#include <cassert>


bool GetVal(MathNode* node, int& value) {
  ConstNode<int>* const_node = dynamic_cast<ConstNode<int>*>(node);
  if (const_node != nullptr) {
    value = const_node->GetValue();
    return true;
  }
  return false;
}

void DeleteRecursive(MathNode* node) {
  if (node == nullptr) return;
  DeleteRecursive(node->GetLeft());
  DeleteRecursive(node->GetRight());
  delete node;
}

MathNode* ReplaceWithRight(MathNode* node) {
  MathNode* new_child = node->GetRight();
  delete node->GetLeft();
  delete node;
  return new_child;
}

MathNode* ReplaceWithLeft(MathNode* node) {
  MathNode* new_child = node->GetLeft();
  delete node->GetRight();
  delete node;
  return new_child;
}

void WriteWithDerivToTex(MathNode* node, FILE* texfile) {
  fprintf(texfile, "\\left(");
  node->WriteToTex(texfile);
  fprintf(texfile, "\\right)_x^{'}");
}


MathNode* OperatorAddNode::DiffThis(const char* var, FILE* texfile) const {
  assert(left_ != nullptr && right_ != nullptr);

  MathNode* left = GetLeft()->Differentiate(var, texfile);
  MathNode* right = GetRight()->Differentiate(var, texfile);
  return *left + *right;
}

void OperatorAddNode::DumpDiffThis(const char* var, FILE* texfile) const {
  WriteWithDerivToTex(GetLeft(), texfile);
  fprintf(texfile, " + ");
  WriteWithDerivToTex(GetRight(), texfile);
}

MathNode* OperatorAddNode::SimplifyThis(FILE* texfile) {
  assert(left_ != nullptr && right_ != nullptr);

  int value = 0;
  if (GetVal(GetLeft(), value) && value == 0) {
    return ReplaceWithRight(this);
  } else
  if (GetVal(GetRight(), value) && value == 0) {
    return ReplaceWithLeft(this);
  }
  return this;
}

void OperatorAddNode::WriteThisToTex(FILE* texfile) const {
  fprintf(texfile, "+");
}

MathNode* OperatorSubNode::DiffThis(const char* var, FILE* texfile) const {
  assert(left_ != nullptr);
  // Note that subtraction can be unary operation.
  if (right_ == nullptr) {
    MathNode* deriv = GetLeft()->Differentiate(var, texfile);
    return -*deriv;
  }
  MathNode* left = GetLeft()->Differentiate(var, texfile);
  MathNode* right = GetRight()->Differentiate(var, texfile);
  return *left - *right;
}

void OperatorSubNode::DumpDiffThis(const char* var, FILE* texfile) const {
  if (right_ == nullptr) {
    fprintf(texfile, "-");
    WriteWithDerivToTex(GetLeft(), texfile);
  } else {
    WriteWithDerivToTex(GetLeft(), texfile);
    fprintf(texfile, " - ");
    WriteWithDerivToTex(GetRight(), texfile);
  }
}

MathNode* OperatorSubNode::SimplifyThis(FILE* texfile) {
  assert(left_ != nullptr);

  int value = 0;
  // Check if subtraction is unary by GetRight().
  if (GetRight() != nullptr && GetVal(GetRight(), value) && value == 0) {
    return ReplaceWithLeft(this);
  } else
  if (GetVal(GetLeft(), value) && value == 0) {
    delete GetLeft();
    ReplaceLeft(GetRight());
    ReplaceRight(nullptr);
  }
  return this;
}

void OperatorSubNode::WriteThisToTex(FILE* texfile) const {
  fprintf(texfile, "-");
}

MathNode* OperatorMulNode::DiffThis(const char* var, FILE* texfile) const {
  assert(left_ != nullptr && right_ != nullptr);

  MathNode* left_mul =
      *GetLeft()->Differentiate(var, texfile) * *GetRight()->GetCopy();
  MathNode* right_mul =
      *GetLeft()->GetCopy() * *GetRight()->Differentiate(var, texfile);
  return *left_mul + *right_mul;
}

void OperatorMulNode::DumpDiffThis(const char* var, FILE* texfile) const {
  WriteWithDerivToTex(GetLeft(), texfile);
  fprintf(texfile, " \\cdot \\left(");
  GetRight()->WriteToTex(texfile);
  fprintf(texfile, "\\right) + \\left(");
  GetLeft()->WriteToTex(texfile);
  fprintf(texfile, "\\right) \\cdot ");
  WriteWithDerivToTex(GetRight(), texfile);
}

MathNode* OperatorMulNode::SimplifyThis(FILE* texfile) {
  assert(left_ != nullptr && right_ != nullptr);

  int value = 0;
  if (GetVal(GetRight(), value) && (value == 0 || value == 1)) {
    if (value == 0) {
      MathNode* right = GetRight();
      DeleteRecursive(GetLeft());
      delete this;
      return right;
    } else {
      return ReplaceWithLeft(this);
    }
  } else
  if (GetVal(GetLeft(), value) && (value == 0 || value == 1)) {
    if (value == 0) {
      MathNode* left = GetLeft();
      DeleteRecursive(GetRight());
      delete this;
      return left;
    } else {
      return ReplaceWithRight(this);
    }
  }
  return this;
}

void OperatorMulNode::WriteThisToTex(FILE* texfile) const {
  fprintf(texfile, "\\cdot");
}

MathNode* OperatorDivNode::DiffThis(const char* var, FILE* texfile) const {
  assert(left_ != nullptr && right_ != nullptr);

  MathNode* lleft_mul =
      *GetLeft()->Differentiate(var, texfile) * *GetRight()->GetCopy();
  MathNode* lright_mul =
      *GetLeft()->GetCopy() * *GetRight()->Differentiate(var, texfile);
  MathNode* left_sub = *lleft_mul - *lright_mul;
  MathNode* right_mul = *GetRight()->GetCopy() * *GetRight()->GetCopy();
  return *left_sub / *right_mul;
}

void OperatorDivNode::DumpDiffThis(const char* var, FILE* texfile) const {
  fprintf(texfile, "\\frac{ ");
  WriteWithDerivToTex(GetLeft(), texfile);
  fprintf(texfile, " \\cdot \\left(");
  GetRight()->WriteToTex(texfile);
  fprintf(texfile, "\\right) - \\left(");
  GetLeft()->WriteToTex(texfile);
  fprintf(texfile, "\\right) \\cdot ");
  WriteWithDerivToTex(GetRight(), texfile);
  fprintf(texfile, " }{\\left(");
  GetRight()->WriteToTex(texfile);
  fprintf(texfile, "\\right)^2}");
}

MathNode* OperatorDivNode::SimplifyThis(FILE* texfile) {
  assert(left_ != nullptr && right_ != nullptr);

  int value = 0;
  if (GetVal(GetRight(), value) && (value == 0 || value == 1)) {
    if (value == 0) {
      assert("Divison by zero! Terminate.");
    } else {
      return ReplaceWithLeft(this);
    }
  } else
  if (GetVal(GetLeft(), value) && value == 0) {
    MathNode* left = GetLeft();
    DeleteRecursive(GetRight());
    delete this;
    return left;
  }
  return this;
}

void OperatorDivNode::WriteThisToTex(FILE* texfile) const {}

void OperatorDivNode::WriteToTex(FILE* texfile) const {
  fprintf(texfile, "\\frac{");
  if (GetLeft()->GetPriority() >= GetPriority()) {
    fprintf(texfile, "\\left(");
  }
  GetLeft()->WriteToTex(texfile);
  if (GetLeft()->GetPriority() >= GetPriority()) {
    fprintf(texfile, "\\right)");
  }
  fprintf(texfile, "}{");
  if (GetPriority() <= GetRight()->GetPriority()) {
    fprintf(texfile, "\\left(");
  }
  GetRight()->WriteToTex(texfile);
  if (GetPriority() <= GetRight()->GetPriority()) {
    fprintf(texfile, "\\right)");
  }
  fprintf(texfile, "}");
}

MathNode* OperatorSinNode::DiffThis(const char* var, FILE* texfile) const {
  assert(left_ != nullptr && right_ == nullptr);

  MathNode* cos = new OperatorCosNode;
  cos->AddChild(GetLeft()->GetCopy());
  return *cos * *GetLeft()->Differentiate(var, texfile);
}

void OperatorSinNode::DumpDiffThis(const char* var, FILE* texfile) const {
  fprintf(texfile, "\\cos\\left(");
  GetLeft()->WriteToTex(texfile);
  fprintf(texfile, "\\right) \\cdot ");
  WriteWithDerivToTex(GetLeft(), texfile);
}

MathNode* OperatorSinNode::SimplifyThis(FILE* texfile) {
  assert(left_ != nullptr && right_ == nullptr);

  int value = 0;
  if (GetVal(GetLeft(), value) && value == 0) {
    return ReplaceWithLeft(this);
  }
  return this;
}

void OperatorSinNode::WriteThisToTex(FILE* texfile) const {
  fprintf(texfile, "\\sin");
}

MathNode* OperatorCosNode::DiffThis(const char* var, FILE* texfile) const {
  assert(left_ != nullptr && right_ == nullptr);

  MathNode* sin = new OperatorSinNode;
  sin->AddChild(GetLeft()->GetCopy());
  return *(-(*sin)) * *GetLeft()->Differentiate(var, texfile);
}

void OperatorCosNode::DumpDiffThis(const char* var, FILE* texfile) const {
  fprintf(texfile, "-\\sin\\left(");
  GetLeft()->WriteToTex(texfile);
  fprintf(texfile, "\\right) \\cdot ");
  WriteWithDerivToTex(GetLeft(), texfile);
}

MathNode* OperatorCosNode::SimplifyThis(FILE* texfile) {
  assert(left_ != nullptr && right_ == nullptr);

  int value = 0;
  if (GetVal(GetLeft(), value) && value == 0) {
    delete GetLeft();
    delete this;
    return new ConstNode<int>(1);
  }
  return this;
}

void OperatorCosNode::WriteThisToTex(FILE* texfile) const {
  fprintf(texfile, "\\cos");
}

MathNode* OperatorTanNode::DiffThis(const char* var, FILE* texfile) const {
  assert(left_ != nullptr && right_ == nullptr);

  MathNode* left_cos = new OperatorCosNode;
  left_cos->AddChild(GetLeft()->GetCopy());
  MathNode* right_cos = left_cos->GetCopy();
  MathNode* one = new ConstNode<int>(1);
  MathNode* tan_derivative = *one / *(*left_cos * *right_cos);
  return *tan_derivative * *GetLeft()->Differentiate(var, texfile);
}

void OperatorTanNode::DumpDiffThis(const char* var, FILE* texfile) const {
  fprintf(texfile, "\\frac{");
  WriteWithDerivToTex(GetLeft(), texfile);
  fprintf(texfile, "}{\\left(\\cos\\left(");
  GetLeft()->WriteToTex(texfile);
  fprintf(texfile, "\\right)\\right)^2}");
}

MathNode* OperatorTanNode::SimplifyThis(FILE* texfile) {
  assert(left_ != nullptr && right_ == nullptr);

  int value = 0;
  if (GetVal(GetLeft(), value) && value == 0) {
    return ReplaceWithLeft(this);
  }
  return this;
}

void OperatorTanNode::WriteThisToTex(FILE* texfile) const {
  fprintf(texfile, "\\tan");
}

MathNode* OperatorCotNode::DiffThis(const char* var, FILE* texfile) const {
  assert(left_ != nullptr && right_ == nullptr);

  MathNode* left_sin = new OperatorSinNode;
  left_sin->AddChild(GetLeft()->GetCopy());
  MathNode* right_sin = left_sin->GetCopy();
  MathNode* min_one = new ConstNode<int>(-1);
  MathNode* cot_derivative = *min_one / *(*left_sin * *right_sin);
  return *cot_derivative * *GetLeft()->Differentiate(var, texfile);
}

void OperatorCotNode::DumpDiffThis(const char* var, FILE* texfile) const {
  fprintf(texfile, "-\\frac{");
  WriteWithDerivToTex(GetLeft(), texfile);
  fprintf(texfile, "}{\\left(\\sin\\left(");
  GetLeft()->WriteToTex(texfile);
  fprintf(texfile, "\\right)\\right)^2}");
}

MathNode* OperatorCotNode::SimplifyThis(FILE* texfile) {
  assert(left_ != nullptr && right_ == nullptr);

  int value = 0;
  if (GetVal(GetLeft(), value) && value == 0) {
    assert("cot(0) is not defined. Terminate.");
  }
  return this;
}

void OperatorCotNode::WriteThisToTex(FILE* texfile) const {
  fprintf(texfile, "\\cot");
}

