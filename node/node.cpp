#include "node.hpp"
#include "string_out.hpp"
#include <cassert>


const size_t MAX_INFO_STR_LEN = 100;

Node::~Node() {
  delete left_;
  delete right_;
}

bool Node::IsLeaf() const {
  return right_ == nullptr;
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

//void Node::DelSon(Node* son) {
//  sons_.erase(son);
//  //for (Node* this_son : sons_) {
//  //  if (son == this_son) {
//  //  }
//  //}
//}

Node* Node::GetLeft() const {
  return left_;
}

Node* Node::GetRight() const {
  return right_;
}

Node* Node::GetParent() const {
  return parent_;
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

const char* Node::GetColor() const {
  return "white";
}

void Node::WriteThisToGraphviz(FILE* graphfile) const {
    fprintf(graphfile, "node%p [label=\"%.*s\" style=filled fillcolor=%s]\n",
        (void*)this,
        UNFOLD(GetString()),
        GetColor());
}

void Node::WriteToGraphviz(FILE* graphfile) const {
  if (GetParent() != nullptr) {
    GetParent()->WriteThisToGraphviz(graphfile);
    fprintf(graphfile, "node%p -> node%p\n", (void*)GetParent(), (void*)this);
  }
  WriteThisToGraphviz(graphfile);
  if (GetLeft() != nullptr) {
    GetLeft()->WriteToGraphviz(graphfile);
  }
  if (GetRight() != nullptr) {
    GetRight()->WriteToGraphviz(graphfile);
  }
}

