#include "node.hpp"
#include "string_out.hpp"
#include <cassert>


const size_t MAX_INFO_STR_LEN = 100;

Node::Node() : left_  (nullptr),
         right_ (nullptr),
         parent_(nullptr) {}

Node::Node(Node* left, Node* right) : Node() {
  AddChild(left);
  AddChild(right);
}

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
}

void Node::ReplaceRight(Node* child) {
  right_ = child;
  if (right_ != nullptr) {
    right_->parent_ = this;
  }
}

void Node::InitParent(Node* parent) {
  assert(parent_ == nullptr);
  parent_ = parent;
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
  WriteThisToGraphviz(graphfile);
  if (GetParent() != nullptr) {
    fprintf(graphfile, "node%p -> node%p\n", (void*)GetParent(), (void*)this);
  }
  if (GetLeft()  != nullptr) { GetLeft()->WriteToGraphviz(graphfile); }
  if (GetRight() != nullptr) { GetRight()->WriteToGraphviz(graphfile); }
}

