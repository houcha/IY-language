#ifndef VAR_NODE_HPP
#define VAR_NODE_HPP

#include "math_node.hpp"


class VarNode : public MathNode {

  private:

    std::string name_;

  public:

    VarNode(const char* str, size_t len) : name_(str, len)    {}
    VarNode(const std::string& str)      : name_(str)         {}
    VarNode(const VarNode& other)        : name_(other.name_) {}

    uint8_t   GetPriority()                  const override;
    MathNode* Differentiate(const char* var) const override;

    const std::string GetString() const override;

  protected:

    MathNode*   CopyThis() const override;
    const char* GetColor() const override;
};



#endif // VAR_NODE_HPP

