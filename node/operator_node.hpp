#ifndef OPERATOR_NODE_HPP
#define OPERATOR_NODE_HPP

#include "math_node.hpp"
#include <string>


/// Ancestor of any operator node.
class OperatorNode : public MathNode {

  protected:

    const char* GetColor() const override { return "gold"; }
};


class OperatorAddNode : public OperatorNode {

  public:

    uint8_t GetPriority() const override { return 2; }
    const std::string GetString() const override { return std::string("+"); }


  protected:

    MathNode* Differentiate(const char* var) const override;
    MathNode* SimplifyThis() override;

    uint32_t  GetThisComplexity() const override { return 1; }
    MathNode* CopyThis() const override { return new OperatorAddNode; }
};


class OperatorSubNode : public OperatorNode {

  public:

    uint8_t GetPriority() const override { return 2; }
    const std::string GetString() const override { return std::string("-"); }


  protected:

    MathNode* Differentiate(const char* var) const override;
    MathNode* SimplifyThis() override;

    uint32_t  GetThisComplexity() const override { return 2; }
    MathNode* CopyThis() const override { return new OperatorSubNode; }
};


class OperatorMulNode : public OperatorNode {

  public:

    uint8_t GetPriority() const override { return 1; }
    const std::string GetString() const override { return std::string("*"); }


  protected:

    MathNode* Differentiate(const char* var) const override;
    MathNode* SimplifyThis() override;

    uint32_t  GetThisComplexity() const override { return 5; }
    MathNode* CopyThis() const override { return new OperatorMulNode; }
};


class OperatorDivNode : public OperatorNode {

  public:

    uint8_t GetPriority() const override { return 1; }
    const std::string GetString() const override { return std::string("/"); }


  protected:

    MathNode* Differentiate(const char* var) const override;
    MathNode* SimplifyThis() override;

    uint32_t  GetThisComplexity() const override { return 6; }
    MathNode* CopyThis() const override { return new OperatorDivNode; }
};


class OperatorSinNode : public OperatorNode {

  public:

    uint8_t GetPriority() const override { return 0; }
    const std::string GetString() const override { return std::string("sin"); }


  protected:

    MathNode* Differentiate(const char* var) const override;
    MathNode* SimplifyThis() override;

    uint32_t  GetThisComplexity() const override { return 10; }
    MathNode* CopyThis() const override { return new OperatorSinNode; }
};


class OperatorCosNode : public OperatorNode {

  public:

    uint8_t GetPriority() const override { return 0; }
    const std::string GetString() const override { return std::string("cos"); }


  protected:

    MathNode* Differentiate(const char* var) const override;
    MathNode* SimplifyThis() override;

    uint32_t  GetThisComplexity() const override { return 10; }
    MathNode* CopyThis() const override { return new OperatorCosNode; }
};


class OperatorTanNode : public OperatorNode {

  public:

    uint8_t GetPriority() const override { return 0; }
    const std::string GetString() const override { return std::string("tan"); }


  protected:

    MathNode* Differentiate(const char* var) const override;
    MathNode* SimplifyThis() override;

    uint32_t  GetThisComplexity() const override { return 15; }
    MathNode* CopyThis() const override { return new OperatorTanNode; }
};


class OperatorCotNode : public OperatorNode {

  public:

    uint8_t GetPriority() const override { return 0; }
    const std::string GetString() const override { return std::string("cot"); }


  protected:

    MathNode* Differentiate(const char* var) const override;
    MathNode* SimplifyThis() override;

    uint32_t  GetThisComplexity() const override { return 15; }
    MathNode* CopyThis() const override { return new OperatorCotNode; }
};


#endif // OPERATOR_NODE_HPP

