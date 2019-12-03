#include "expression_tree.hpp"


ExprTree::ExprTree(MathNode* node) {
  root_ = node;
  for (auto it = Begin(); it != End(); ++it) {
    ++count_nodes_;
  }
}

void ExprTree::AddChild(value_type* parent, value_type* son) {
  parent->AddChild(son);
  ++count_nodes_;
}

ExprTree ExprTree::Differentiate(const char* var) const {
  return ExprTree(root_->Differentiate(var));
}

void ExprTree::ReplaceNode(MathNode* replaced, MathNode* new_node) {
  if (replaced == root_) {
    root_ = new_node;
  } else {
    // Fix parent.
    if (replaced->IsLeftChild()) {
      replaced->GetParent()->ReplaceLeft(new_node);
    } else {
      replaced->GetParent()->ReplaceRight(new_node);
    }
  }
  // Fix node.
  new_node->AddChild(replaced->GetLeft());
  new_node->AddChild(replaced->GetRight());
}

