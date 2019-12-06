#ifndef EXPRESSION_TREE_HPP
#define EXPRESSION_TREE_HPP

#include "binary_tree.hpp"
#include "node/math_node.hpp"


class ExprTree : BinaryTree<MathNode> {

  public:

    typedef BinaryTree<MathNode>::value_type value_type;

    ExprTree() {}
    ExprTree(MathNode* node);
    template <typename String>
    ExprTree(const String& buffer);
    ~ExprTree() = default;

          value_type* GetRoot()       { return root_; }
    const value_type* GetRoot() const { return root_; }

    template <typename IteratorType = PreOrderIterator<value_type>>
    IteratorType Begin();
    template <typename IteratorType = PreOrderIterator<value_type>>
    IteratorType End();
    template <typename IteratorType = PreOrderIterator<value_type>>
    IteratorType Begin() const;
    template <typename IteratorType = PreOrderIterator<value_type>>
    IteratorType End()   const;

    ExprTree Differentiate(const char* var, FILE* texfile) const;
    /// Dumps tree in graphviz.
    void Dump(FILE* graphfile) const;

    void AddChild(value_type* parent, value_type* son);

  private:

    template <typename String>
    void ParseInput(const String& buffer);
    /// Change `replaced` node to `new_node` in the tree.
    /// Do not delete old node.
    void ReplaceNode(MathNode* replaced, MathNode* new_node);
};

#include "expression_tree.inl"


#endif // EXPRESSION_TREE_HPP

