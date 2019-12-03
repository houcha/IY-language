#ifndef OPERATOR_NODE_HPP
#define OPERATOR_NODE_HPP

#include "math_node.hpp"
#include <string>


/// Ancestor of any operator node.
class OperatorNode : public MathNode {
  protected:
    const char* GetColor() const override { return "gold"; }
};


class OperatorCosNode : public OperatorNode {

  public:

    MathNode* Differentiate(const char* var) const override;

  protected:

    MathNode*         CopyThis()  const override { return new OperatorCosNode; }
    const std::string GetString() const override { return std::string("cos"); }
};


class OperatorTanNode : public OperatorNode {

  public:

    MathNode* Differentiate(const char* var) const override;

  protected:

    MathNode*         CopyThis()  const override { return new OperatorTanNode; }
    const std::string GetString() const override { return std::string("tan"); }
};


class OperatorAddNode : public OperatorNode {

  public:

    MathNode* Differentiate(const char* var) const override;

  protected:

    MathNode*         CopyThis()  const override { return new OperatorAddNode; }
    const std::string GetString() const override { return std::string("+"); }
};


class OperatorMulNode : public OperatorNode {

  public:

    MathNode* Differentiate(const char* var) const override;

  protected:

    MathNode*         CopyThis()  const override { return new OperatorMulNode; }
    const std::string GetString() const override { return std::string("*"); }
};


class OperatorSubNode : public OperatorNode {

  public:

    MathNode* Differentiate(const char* var) const override;

  protected:

    MathNode*         CopyThis()  const override { return new OperatorSubNode; }
    const std::string GetString() const override { return std::string("-"); }
};


class OperatorSinNode : public OperatorNode {

  public:

    MathNode* Differentiate(const char* var) const override;

  protected:

    MathNode*         CopyThis()  const override { return new OperatorSinNode; }
    const std::string GetString() const override { return std::string("sin"); }
};


class OperatorDivNode : public OperatorNode {

  public:

    MathNode* Differentiate(const char* var) const override;

  protected:

    MathNode*         CopyThis()  const override { return new OperatorDivNode; }
    const std::string GetString() const override { return std::string("/"); }
};


class OperatorCotNode : public OperatorNode {

  public:

    MathNode* Differentiate(const char* var) const override;

  protected:

    MathNode*         CopyThis()  const override { return new OperatorCotNode; }
    const std::string GetString() const override { return std::string("cot"); }
};


#endif // OPERATOR_NODE_HPP

