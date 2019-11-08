#ifndef NODE_HPP
#define NODE_HPP

#include <string_view>


struct QueryNode {

    std::string_view query_;

    QueryNode* left_;
    QueryNode* right_;
    QueryNode* parent_;

    QueryNode()
        : query_ (),
          left_  (nullptr),
          right_ (nullptr),
          parent_(nullptr) {}

    bool IsLeaf() const { return left_ == nullptr && right_ == nullptr; }
    bool IsRoot() const { return parent_ == nullptr; }
};


#endif // NODE_HPP


