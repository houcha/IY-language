#ifndef VAR_NODE_HPP
#define VAR_NODE_HPP

#include "math_node.hpp"


class VarNode : public MathNode {

  private:

    const std::string name_;

  public:

    VarNode(const char* str, size_t len) : name_(str, len)    {}
    VarNode(const VarNode& other)        : name_(other.name_) {}

    MathNode* Differentiate(const char* var) const override;

  protected:

    MathNode*         CopyThis()  const override;
    const std::string GetString() const override;
    const char*       GetColor()  const override;
};



#endif // VAR_NODE_HPP

