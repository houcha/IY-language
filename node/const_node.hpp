#ifndef CONST_NODE_HPP
#define CONST_NODE_HPP

#include "math_node.hpp"


template <typename T>
class ConstNode : public MathNode {

  private:

    T value_;

  public:

    ConstNode(T value)                : value_(value)        { Update(); }
    ConstNode(const ConstNode& other) : value_(other.value_) { Update(); }

    uint8_t GetPriority() const override;
    T       GetValue()    const;

  protected:

    void DumpDiffThis(const char* var, FILE* texfile) const override;

    MathNode*         DiffThis(const char* var, FILE* texfile) const override;
    MathNode*         CopyThis()          const override;
    uint32_t          GetThisComplexity() const override;
    const std::string GetString()         const override;
    const char*       GetColor()          const override;
};

#include "const_node.inl"


#endif // CONST_NODE_HPP

