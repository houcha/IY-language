#ifndef VAR_NODE_HPP
#define VAR_NODE_HPP

#include "math_node.hpp"


class VarNode : public MathNode {

  private:

    const std::string name_;

  public:

    VarNode(const char* str, size_t len) : name_(str, len)    { Update(); }
    VarNode(const VarNode& other)        : name_(other.name_) { Update(); }

    uint8_t GetPriority() const override;

  protected:

    void DumpDiffThis(const char* var, FILE* texfile) const override;
    MathNode*         DiffThis(const char* var, FILE* texfile) const override;
    MathNode*         CopyThis()          const override;
    uint32_t          GetThisComplexity() const override;
    const std::string GetString()         const override;
    const char*       GetColor()          const override;
};



#endif // VAR_NODE_HPP

