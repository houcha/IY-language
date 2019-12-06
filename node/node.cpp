#include "node.hpp"
#include <cassert>


bool Node::IsLeaf() const {
  return left_ == nullptr && right_ == nullptr;
}

bool Node::IsRoot() const {
  return parent_ == nullptr;
}

bool Node::IsLeftChild() const {
  return parent_ != nullptr && this == parent_->left_;
}

bool Node::IsRightChild() const {
  return parent_ != nullptr && this == parent_->right_;
}

void Node::AddChild(Node* child) {
  if (child != nullptr) {
    child->parent_ = this;
  }
  if (left_ == nullptr) {
    left_ = child;
  } else
  if (right_ == nullptr) {
    right_ = child;
  } else {
    assert("Cannot add child: both of children exist");
  }
  Update();
}

void Node::ReplaceLeft(Node* child) {
  left_ = child;
  if (left_ != nullptr) {
    left_->parent_ = this;
  }
  Update();
}

void Node::ReplaceRight(Node* child) {
  right_ = child;
  if (right_ != nullptr) {
    right_->parent_ = this;
  }
  Update();
}

