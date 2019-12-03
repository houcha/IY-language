#ifndef CONST_NODE_HPP
#define CONST_NODE_HPP

#include "math_node.hpp"


template <typename T>
class ConstNode : public MathNode {

  private:

    T value_;

  public:

    ConstNode(T value)                : value_(value)        {}
    ConstNode(const ConstNode& other) : value_(other.value_) {}

    MathNode* Differentiate(const char* var) const override;

  protected:

    MathNode*         CopyThis()  const override;
    const std::string GetString() const override;
    const char*       GetColor()  const override;
};

#include "const_node.inl"


#endif // CONST_NODE_HPP

