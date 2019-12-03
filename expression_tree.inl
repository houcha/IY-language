#include <stack>
#include "node/create_node.hpp"


template <typename String>
ExprTree::ExprTree(const String& buffer) {
  ParseInput(buffer);
}

template <typename String>
void ExprTree::ParseInput(const String& buffer) {
  std::stack<MathNode*> unassigned_nodes;

  const char* lexeme_begin = buffer.data();
  const char* lexeme_end   = lexeme_begin;
  const char* buffer_end = lexeme_begin + buffer.size();

  for (const char* current = lexeme_begin; current < buffer_end; ++current) {
    if (*current == '(' || *current == ')') {
      // Add lexeme.
      lexeme_end = current - 1;
      if (lexeme_end - lexeme_begin >= 0) {
        MathNode* tmp = unassigned_nodes.top();
        unassigned_nodes.pop();
        MathNode* new_node = CreateNode(lexeme_begin, lexeme_end);
        unassigned_nodes.push(new_node);
        if (new_node != nullptr) {
          ReplaceNode(tmp, new_node);
        }
        delete tmp; // Delete old (temporary) node.
      }
      // Create or delete tree node.
      if (*current == '(') {
        if (unassigned_nodes.empty()) {
          root_ = new EmptyNode;
          unassigned_nodes.push(root_);
        } else {
          // Create empty node, since the real type of MathNode is unknown
          // until input string is given.
          MathNode* tmp = new EmptyNode;
          AddChild(unassigned_nodes.top(), tmp);
          unassigned_nodes.push(tmp);
        }
      } else { // *current == ')'
        unassigned_nodes.pop();
      }
      lexeme_begin = current + 1;
    }
  }
}

template <typename IteratorType>
IteratorType ExprTree::Begin() {
  return BinaryTree<MathNode>::Begin<IteratorType>();
}

template <typename IteratorType>
IteratorType ExprTree::End() {
  return BinaryTree<MathNode>::End<IteratorType>();
}

template <typename IteratorType>
IteratorType ExprTree::Begin() const {
  return BinaryTree<MathNode>::Begin<IteratorType>();
}

template <typename IteratorType>
IteratorType ExprTree::End() const {
  return BinaryTree<MathNode>::End<IteratorType>();
}

