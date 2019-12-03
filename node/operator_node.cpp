#include "operator_node.hpp"
#include "const_node.hpp"
#include "operators.hpp"
#include <cassert>


MathNode* OperatorAddNode::Differentiate(const char* var) const {
  assert(left_ != nullptr && right_ != nullptr);
  return *GetLeft()->Differentiate(var) + *GetRight()->Differentiate(var);
}

MathNode* OperatorSubNode::Differentiate(const char* var) const {
  assert(left_ != nullptr && right_ != nullptr);
  return *GetLeft()->Differentiate(var) - *GetRight()->Differentiate(var);
}

MathNode* OperatorMulNode::Differentiate(const char* var) const {
  assert(left_ != nullptr && right_ != nullptr);

  MathNode* left_mul = *GetLeft()->Differentiate(var) * *GetRight()->GetCopy();
  MathNode* right_mul = *GetLeft()->GetCopy() * *GetRight()->Differentiate(var);
  return *left_mul + *right_mul;;
}

MathNode* OperatorDivNode::Differentiate(const char* var) const {
  assert(left_ != nullptr && right_ != nullptr);

  MathNode* lleft_mul = *GetLeft()->Differentiate(var) * *GetRight()->GetCopy();
  MathNode* lright_mul =
      *GetLeft()->GetCopy() * *GetRight()->Differentiate(var);
  MathNode* left_sub = *lleft_mul - *lright_mul;
  MathNode* right_mul = *GetRight()->GetCopy() * *GetRight()->GetCopy();
  return *left_sub / *right_mul;
}

MathNode* OperatorSinNode::Differentiate(const char* var) const {
  assert(left_ != nullptr && right_ == nullptr);

  MathNode* cos = new OperatorCosNode;
  cos->AddChild(GetLeft()->GetCopy());
  return *cos * *GetLeft()->Differentiate(var);
}

MathNode* OperatorCosNode::Differentiate(const char* var) const {
  assert(left_ != nullptr && right_ == nullptr);

  MathNode* sin = new OperatorSinNode;
  sin->AddChild(GetLeft()->GetCopy());
  return *(-(*sin)) * *GetLeft()->Differentiate(var);
}

MathNode* OperatorTanNode::Differentiate(const char* var) const {
  assert(left_ != nullptr && right_ == nullptr);

  MathNode* left_cos = new OperatorCosNode;
  left_cos->AddChild(GetLeft()->GetCopy());
  MathNode* right_cos = left_cos->GetCopy();
  MathNode* one = new ConstNode<int>(1);
  MathNode* tan_derivative = *one / *(*left_cos * *right_cos);
  return *tan_derivative * *GetLeft()->Differentiate(var);
}

MathNode* OperatorCotNode::Differentiate(const char* var) const {
  assert(left_ != nullptr && right_ == nullptr);

  MathNode* left_sin = new OperatorSinNode;
  left_sin->AddChild(GetLeft()->GetCopy());
  MathNode* right_sin = left_sin->GetCopy();
  MathNode* min_one = new ConstNode<int>(-1);
  MathNode* cot_derivative = *min_one / *(*left_sin * *right_sin);
  return *cot_derivative * *GetLeft()->Differentiate(var);
}

