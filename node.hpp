#ifndef NODE_HPP
#define NODE_HPP


struct MathNode {

    MathNode* left_;
    MathNode* right_;
    MathNode* parent_;

    MathNode() : left_  (nullptr),
                 right_ (nullptr),
                 parent_(nullptr) {}

    bool IsLeaf()       const
    { return left_ == nullptr && right_ == nullptr; }

    bool IsRoot()       const
    { return parent_ == nullptr; }

    bool IsLeftChild()  const
    { return parent_ != nullptr && this == parent_->left_; }

    bool IsRightChild() const
    { return parent_ != nullptr && this == parent_->right_; }
};


template <typename T>
struct ConstNode : MathNode {

    T value_;

    ConstNode(T value) : MathNode(),
                         value_  (value) {}
};


struct OperationNode : MathNode {

    char symbol_;

    OperationNode(char symbol) : MathNode(),
                                 symbol_ () {}
};


#endif // NODE_HPP


