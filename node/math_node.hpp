#ifndef MATH_NODE_HPP
#define MATH_NODE_HPP

#include "node.hpp"


struct NodeInfo {

    uint32_t complexity_;

    NodeInfo()
        : complexity_(0) {}

    NodeInfo(const NodeInfo& other)
        : complexity_(other.complexity_) {}
};


class MathNode : public Node {

  protected:

    NodeInfo info_;

  public:

    MathNode()                      : info_() {}
    MathNode(const MathNode& other) : info_(other.info_) {}

    virtual MathNode* Differentiate(const char* var) const = 0;
    virtual uint8_t   GetPriority() const = 0;

    MathNode* GetLeft()   const;
    MathNode* GetRight()  const;
    MathNode* GetParent() const;

    /// Return relative complexity of a node.
    uint32_t GetComplexity() const;
    /// Return dynamically created node which is copy of this one.
    MathNode* GetCopy() const;
    MathNode* Simplify();

  protected:

    virtual MathNode*         CopyThis()          const = 0;
    virtual uint32_t          GetThisComplexity() const = 0;
    virtual const char*       GetColor()          const;
    virtual MathNode*         SimplifyThis();
    virtual void              Update();
};


/// Helper class. Used if it is neccessary to create MathNode of unknown type.
/// (Note that MathNode is pure virtual, so it cannot be created.)
class EmptyNode : public MathNode {

  public:

    uint8_t GetPriority() const override { return 0; }

    MathNode* Differentiate(const char* var) const override;

    const std::string GetString() const override { return std::string(); }

  protected:

    uint32_t          GetThisComplexity() const override { return 0; }
    MathNode*         CopyThis()  const override { return new EmptyNode; }
};


#endif // MATH_NODE_HPP

