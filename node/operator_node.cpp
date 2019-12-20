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


MathNode* OperatorAddNode::Differentiate(const char* var) const {
  assert(GetLeft() != nullptr && GetRight() != nullptr);

  MathNode* left = GetLeft()->Differentiate(var);
  MathNode* right = GetRight()->Differentiate(var);
  return *left + *right;
}

MathNode* OperatorAddNode::SimplifyThis() {
  assert(GetLeft() != nullptr && GetRight() != nullptr);

  int value = 0;
  if (GetVal(GetLeft(), value) && value == 0) {
    return ReplaceWithRight(this);
  } else
  if (GetVal(GetRight(), value) && value == 0) {
    return ReplaceWithLeft(this);
  }
  return this;
}

MathNode* OperatorSubNode::Differentiate(const char* var) const {
  assert(GetLeft() != nullptr);
  // Note that subtraction can be unary operation.
  if (GetRight() == nullptr) {
    MathNode* deriv = GetLeft()->Differentiate(var);
    return -*deriv;
  }
  MathNode* left = GetLeft()->Differentiate(var);
  MathNode* right = GetRight()->Differentiate(var);
  return *left - *right;
}

MathNode* OperatorSubNode::SimplifyThis() {
  assert(GetLeft() != nullptr);

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

MathNode* OperatorMulNode::Differentiate(const char* var) const {
  assert(GetLeft() != nullptr && GetRight() != nullptr);

  MathNode* left_mul =
      *GetLeft()->Differentiate(var) * *GetRight()->GetCopy();
  MathNode* right_mul =
      *GetLeft()->GetCopy() * *GetRight()->Differentiate(var);
  return *left_mul + *right_mul;
}

MathNode* OperatorMulNode::SimplifyThis() {
  assert(GetLeft() != nullptr && GetRight() != nullptr);

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

MathNode* OperatorDivNode::Differentiate(const char* var) const {
  assert(GetLeft() != nullptr && GetRight() != nullptr);

  MathNode* lleft_mul =
      *GetLeft()->Differentiate(var) * *GetRight()->GetCopy();
  MathNode* lright_mul =
      *GetLeft()->GetCopy() * *GetRight()->Differentiate(var);
  MathNode* left_sub = *lleft_mul - *lright_mul;
  MathNode* right_mul = *GetRight()->GetCopy() * *GetRight()->GetCopy();
  return *left_sub / *right_mul;
}

MathNode* OperatorDivNode::SimplifyThis() {
  assert(GetLeft() != nullptr && GetRight() != nullptr);

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

MathNode* OperatorSinNode::Differentiate(const char* var) const {
  assert(GetLeft() != nullptr && GetRight() == nullptr);

  MathNode* cos = new OperatorCosNode;
  cos->AddChild(GetLeft()->GetCopy());
  return *cos * *GetLeft()->Differentiate(var);
}

MathNode* OperatorSinNode::SimplifyThis() {
  assert(GetLeft() != nullptr && GetRight() == nullptr);

  int value = 0;
  if (GetVal(GetLeft(), value) && value == 0) {
    return ReplaceWithLeft(this);
  }
  return this;
}

MathNode* OperatorCosNode::Differentiate(const char* var) const {
  assert(GetLeft() != nullptr && GetRight() == nullptr);

  MathNode* sin = new OperatorSinNode;
  sin->AddChild(GetLeft()->GetCopy());
  return *(-(*sin)) * *GetLeft()->Differentiate(var);
}

MathNode* OperatorCosNode::SimplifyThis() {
  assert(GetLeft() != nullptr && GetRight() == nullptr);

  int value = 0;
  if (GetVal(GetLeft(), value) && value == 0) {
    delete GetLeft();
    delete this;
    return new ConstNode<int>(1);
  }
  return this;
}

MathNode* OperatorTanNode::Differentiate(const char* var) const {
  assert(GetLeft() != nullptr && GetRight() == nullptr);

  MathNode* left_cos = new OperatorCosNode;
  left_cos->AddChild(GetLeft()->GetCopy());
  MathNode* right_cos = left_cos->GetCopy();
  MathNode* one = new ConstNode<int>(1);
  MathNode* tan_derivative = *one / *(*left_cos * *right_cos);
  return *tan_derivative * *GetLeft()->Differentiate(var);
}

MathNode* OperatorTanNode::SimplifyThis() {
  assert(GetLeft() != nullptr && GetRight() == nullptr);

  int value = 0;
  if (GetVal(GetLeft(), value) && value == 0) {
    return ReplaceWithLeft(this);
  }
  return this;
}

MathNode* OperatorCotNode::Differentiate(const char* var) const {
  assert(GetLeft() != nullptr && GetRight() == nullptr);

  MathNode* left_sin = new OperatorSinNode;
  left_sin->AddChild(GetLeft()->GetCopy());
  MathNode* right_sin = left_sin->GetCopy();
  MathNode* min_one = new ConstNode<int>(-1);
  MathNode* cot_derivative = *min_one / *(*left_sin * *right_sin);
  return *cot_derivative * *GetLeft()->Differentiate(var);
}

MathNode* OperatorCotNode::SimplifyThis() {
  assert(GetLeft() != nullptr && GetRight() == nullptr);

  int value = 0;
  if (GetVal(GetLeft(), value) && value == 0) {
    assert("cot(0) is not defined. Terminate.");
  }
  return this;
}

