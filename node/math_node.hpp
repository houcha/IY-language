#ifndef MATH_NODE_HPP
#define MATH_NODE_HPP

#include "node.hpp"


class MathNode : public Node {

  public:

    MathNode() = default;

    virtual MathNode* Differentiate(const char* var) const = 0;
    virtual uint8_t   GetPriority()                  const = 0;

    MathNode* GetLeft()   const;
    MathNode* GetRight()  const;
    MathNode* GetParent() const;

    /// Return dynamically created node which is copy of this one.
    MathNode* GetCopy() const;
    MathNode* Simplify();

  protected:

    virtual MathNode*   CopyThis()          const = 0;
    virtual const char* GetColor()          const;
    virtual MathNode*   SimplifyThis();
};


/// Helper class. Used if it is neccessary to create MathNode of unknown type.
/// (Note that MathNode is pure virtual, so it cannot be created.)
class EmptyNode : public MathNode {

  public:

    uint8_t GetPriority() const override { return 0; }

    MathNode* Differentiate(const char* var) const override;

    const std::string GetString() const override { return std::string(); }

  protected:

    MathNode* CopyThis()          const override { return new EmptyNode; }
};


#endif // MATH_NODE_HPP

