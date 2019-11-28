#ifndef BINARY_TREE_HPP
#define BINARY_TREE_HPP

#include "binary_tree_it.hpp"


template <typename Node_t>
class BinaryTree {

  public:

    typedef Node_t value_type;

  protected:

    value_type* root_;
    /// Total number of nodes.
    size_t count_nodes_;

  public:

    BinaryTree()
        : root_       (new Node_t),
          count_nodes_(1) {}

   ~BinaryTree();

    size_t      Size()    { return count_nodes_; }
    value_type* GetRoot() { return root_; }

    template <typename IteratorType = PreOrderIterator<value_type>>
    IteratorType Begin() { return IteratorType(root_); }
    template <typename IteratorType = PreOrderIterator<value_type>>
    IteratorType End()   { return IteratorType(nullptr); }
    template <typename IteratorType = PreOrderIterator<value_type>>
    IteratorType Begin() const { return IteratorType(root_); }
    template <typename IteratorType = PreOrderIterator<value_type>>
    IteratorType End()   const { return IteratorType(nullptr); }
};

#include "binary_tree.inl"


#endif // BINARY_TREE_HPP

