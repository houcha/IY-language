#ifndef EXPRESSION_TREE_HPP
#define EXPRESSION_TREE_HPP

#include "binary_tree.hpp"
#include "node.hpp"


class ExprTree : BinaryTree<Node> {

  public:

    typedef BinaryTree<Node>::value_type value_type;

    ExprTree() : BinaryTree<Node>() {}
   ~ExprTree() = default;

          value_type* GetRoot()       { return this->root_; }
    const value_type* GetRoot() const { return this->root_; }



    template <typename IteratorType = PreOrderIterator<value_type>>
    IteratorType Begin();
    template <typename IteratorType = PreOrderIterator<value_type>>
    IteratorType End();
    template <typename IteratorType = PreOrderIterator<value_type>>
    IteratorType Begin() const;
    template <typename IteratorType = PreOrderIterator<value_type>>
    IteratorType End()   const;

    ExprTree Differentiate() const;

  private:

    void AddChild(Node* parent, Node* son) {
      if (parent->right_ == nullptr) {
        parent->right_ = son;
      } else {
        parent->left_ = son;
      }
      son->parent_ = parent;
      ++count_nodes_;
    }
};


#endif // EXPRESSION_TREE_HPP

