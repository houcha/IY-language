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


  protected:

    void DumpDiffThis(const char* var, FILE* texfile) const override;
    MathNode*         DiffThis(const char* var, FILE* texfile) const override;
    void              WriteThisToTex(FILE* texfile)            const override;
    MathNode*         SimplifyThis(FILE* texfile)                    override;

    uint32_t          GetThisComplexity() const override { return 1; }
    MathNode*         CopyThis()  const override { return new OperatorAddNode; }
    const std::string GetString() const override { return std::string("+"); }
};


class OperatorSubNode : public OperatorNode {

  public:

    uint8_t GetPriority() const override { return 2; }


  protected:

    void DumpDiffThis(const char* var, FILE* texfile) const override;
    MathNode*         DiffThis(const char* var, FILE* texfile) const override;
    void              WriteThisToTex(FILE* texfile)            const override;
    MathNode*         SimplifyThis(FILE* texfile)                    override;

    uint32_t          GetThisComplexity() const override { return 2; }
    MathNode*         CopyThis()  const override { return new OperatorSubNode; }
    const std::string GetString() const override { return std::string("-"); }
};


class OperatorMulNode : public OperatorNode {

  public:

    uint8_t GetPriority() const override { return 1; }


  protected:

    void DumpDiffThis(const char* var, FILE* texfile) const override;
    MathNode*         DiffThis(const char* var, FILE* texfile) const override;
    void              WriteThisToTex(FILE* texfile)            const override;
    MathNode*         SimplifyThis(FILE* texfile)                    override;

    uint32_t          GetThisComplexity() const override { return 5; }
    MathNode*         CopyThis()  const override { return new OperatorMulNode; }
    const std::string GetString() const override { return std::string("*"); }
};


class OperatorDivNode : public OperatorNode {

  public:

    uint8_t GetPriority() const override { return 1; }


    void WriteToTex(FILE* texfile) const override;

  protected:

    void DumpDiffThis(const char* var, FILE* texfile) const override;
    MathNode*         DiffThis(const char* var, FILE* texfile) const override;
    void              WriteThisToTex(FILE* texfile)            const override;
    MathNode*         SimplifyThis(FILE* texfile)                    override;

    uint32_t          GetThisComplexity() const override { return 6; }
    MathNode*         CopyThis()  const override { return new OperatorDivNode; }
    const std::string GetString() const override { return std::string("/"); }
};


class OperatorSinNode : public OperatorNode {

  public:

    uint8_t GetPriority() const override { return 0; }


  protected:

    void DumpDiffThis(const char* var, FILE* texfile) const override;
    MathNode*         DiffThis(const char* var, FILE* texfile) const override;
    void              WriteThisToTex(FILE* texfile)            const override;
    MathNode*         SimplifyThis(FILE* texfile)                    override;

    uint32_t          GetThisComplexity() const override { return 10; }
    MathNode*         CopyThis()  const override { return new OperatorSinNode; }
    const std::string GetString() const override { return std::string("sin"); }
};


class OperatorCosNode : public OperatorNode {

  public:

    uint8_t GetPriority() const override { return 0; }


  protected:

    void DumpDiffThis(const char* var, FILE* texfile) const override;
    MathNode*         DiffThis(const char* var, FILE* texfile) const override;
    void              WriteThisToTex(FILE* texfile)            const override;
    MathNode*         SimplifyThis(FILE* texfile)                    override;

    uint32_t          GetThisComplexity() const override { return 10; }
    MathNode*         CopyThis()  const override { return new OperatorCosNode; }
    const std::string GetString() const override { return std::string("cos"); }
};


class OperatorTanNode : public OperatorNode {

  public:

    uint8_t GetPriority() const override { return 0; }


  protected:

    void DumpDiffThis(const char* var, FILE* texfile) const override;
    MathNode*         DiffThis(const char* var, FILE* texfile) const override;
    void              WriteThisToTex(FILE* texfile)            const override;
    MathNode*         SimplifyThis(FILE* texfile)                    override;

    uint32_t          GetThisComplexity() const override { return 15; }
    MathNode*         CopyThis()  const override { return new OperatorTanNode; }
    const std::string GetString() const override { return std::string("tan"); }
};


class OperatorCotNode : public OperatorNode {

  public:

    uint8_t GetPriority() const override { return 0; }


  protected:

    void DumpDiffThis(const char* var, FILE* texfile) const override;
    MathNode*         DiffThis(const char* var, FILE* texfile) const override;
    void              WriteThisToTex(FILE* texfile)            const override;
    MathNode*         SimplifyThis(FILE* texfile)                    override;

    uint32_t          GetThisComplexity() const override { return 15; }
    MathNode*         CopyThis()  const override { return new OperatorCotNode; }
    const std::string GetString() const override { return std::string("cot"); }
};


#endif // OPERATOR_NODE_HPP

