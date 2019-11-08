#ifndef DECISION_TREE_HPP
#define DECISION_TREE_HPP

#include <stack>
#include "node.hpp"
#include "binary_tree.hpp"


class DecisionTree : BinaryTree<QueryNode> {

  public:

    template <typename String>
    DecisionTree(const String& buffer);
   ~DecisionTree() = default;

    template <typename IteratorType = PreOrderIterator<value_type>>
    IteratorType Begin() { BinaryTree<QueryNode>::Begin<IteratorType>(); }
    template <typename IteratorType = PreOrderIterator<value_type>>
    IteratorType End() { BinaryTree<QueryNode>::End<IteratorType>(); }

  private:

    void AddNode(QueryNode* parent, QueryNode* son);
};

#include "decision_tree.inl"


#endif // DECISION_TREE_HPP

