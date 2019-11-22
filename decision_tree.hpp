#ifndef DECISION_TREE_HPP
#define DECISION_TREE_HPP

#include "binary_tree.hpp"
#include "node.hpp"


class DecisionTree : BinaryTree<QueryNode> {

  public:

    typedef BinaryTree<QueryNode>::value_type value_type;

    DecisionTree() : BinaryTree() {}
    template <typename String>
    DecisionTree(const String& buffer);
   ~DecisionTree() = default;

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

    void AddChild(QueryNode* parent, QueryNode* son) {
      if (parent->right_ == nullptr) {
        parent->right_ = son;
      } else {
        parent->left_ = son;
      }
      son->parent_ = parent;

      ++count_nodes_;
    }


  private:

    template <typename String>
    void ParseString(const String& buffer);
};




#include "decision_tree.inl"

#endif // DECISION_TREE_HPP

