#ifndef MATH_NODE_HPP
#define MATH_NODE_HPP

#include "node.hpp"
#include <string>


class MathNode : public Node {

  public:

    virtual ~MathNode() = default;

    virtual MathNode* Differentiate(const char* var) const = 0;
    /// Return dynamically created node which is copy of this one.
    MathNode* GetCopy() const;

    virtual const std::string GetGraphvizInfo() const;

    MathNode* GetLeft()   const;
    MathNode* GetRight()  const;
    MathNode* GetParent() const;

  protected:

    virtual MathNode*         CopyThis()  const = 0;
    virtual const std::string GetString() const = 0;
    virtual const char*       GetColor()  const;
};


/// Helper class. Used if it is neccessary to create MathNode of unknown type.
/// (Note that MathNode is pure virtual, so it cannot be created.)
class EmptyNode : public MathNode {

  public:

    MathNode* Differentiate(const char* var) const override {
      return new EmptyNode;
    }

  protected:

    MathNode*         CopyThis()  const override { return new EmptyNode; }
    const std::string GetString() const override { return std::string(); }
};


#endif // MATH_NODE_HPP

