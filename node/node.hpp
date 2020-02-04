#ifndef NODE_HPP
#define NODE_HPP

#include <cstdio>
#include <vector>
#include <string>


class Node {

  protected:

    Node* left_;
    Node* right_;
    Node* parent_;

  public:

    Node();
    Node(Node* left, Node* right);

    virtual ~Node();

    bool IsLeaf()       const;
    bool IsRoot()       const;
    bool IsLeftChild()  const;
    bool IsRightChild() const;

    void AddChild(Node* child);

    void ReplaceLeft (Node* child);
    void ReplaceRight(Node* child);

    void InitParent(Node* parent);

    Node* GetLeft()   const;
    Node* GetRight()  const;
    Node* GetParent() const;

    virtual void WriteToGraphviz(FILE* graphfile) const;

    virtual const std::string GetString() const = 0;

  protected:

    virtual const char* GetColor() const;

    void WriteThisToGraphviz(FILE* graphfile) const;
};


class FuncDeclNode : public Node {

  private:

    // left_ is args.
    // right_ is body.
    std::string name_;

  public:

    FuncDeclNode(const std::string& name, Node* args, Node* statements)
        : Node(args, statements),
          name_(name) {}

    const std::string GetString() const override { return name_; }
    const char*       GetColor()  const override { return "aquamarine"; }
};


class FuncCallNode : public Node {

  private:

    // left_ is args.
    // right_ is body.
    std::string name_;

  public:

    FuncCallNode(const std::string& name, Node* args)
        : Node(args, nullptr),
          name_(name) {}

    const std::string GetString() const override { return name_; }
    const char*       GetColor()  const override { return "aquamarine"; }
};


class IdentifierNode : public Node {

  private:

    std::string name_;

  public:

    IdentifierNode(const std::string& name) : name_(name) {}

    const std::string GetString() const override { return name_; }
};


class AssignNode : public Node {

  public:

    AssignNode(Node* var, Node* expr) : Node(var, expr) {}

    const std::string GetString() const override { return "="; }
};

class LoopNode : public Node {

  public:

    LoopNode(Node* condition, Node* statement) : Node(condition, statement) {}

    const std::string GetString() const override { return "LOOP"; }
};

class IfNode : public Node {

  public:

    IfNode(Node* condition, Node* statement) : Node(condition, statement) {}

    const std::string GetString() const override { return "IF"; }
};

//------------------------------------------------------------------------------

class MultiNode : public Node {

  protected:

    std::vector<Node*> sons_;

  public:

    void WriteToGraphviz(FILE* graphfile) const override {
      WriteThisToGraphviz(graphfile);
      if (GetParent() != nullptr) {
        fprintf(graphfile, "node%p -> node%p\n", (void*)GetParent(), (void*)this);
      }
      for (Node* son : sons_) {
        son->WriteToGraphviz(graphfile);
      }
    }

    void AddSon(Node* son) {
      sons_.push_back(son);
      son->InitParent(this);
    }
};

class BaseNode : public MultiNode {

  public:

    void AddDecl(Node* node) { AddSon(node); }

    const std::string GetString() const override { return "BASE"; }
    const char*       GetColor()  const override { return "black"; }
};

class VarsNode : public MultiNode {

  public:

    void AddVar(Node* node) { AddSon(node); }

    const std::string GetString() const override { return "VARS"; }
};

class ArgsNode : public MultiNode {

  public:

    void AddArg(Node* node) { AddSon(node); }

    const std::string GetString() const override { return "ARGS"; }
};


class StatementNode : public MultiNode {

  public:

    void AddStatement(Node* node) { AddSon(node); }

    const std::string GetString() const override { return "STAT"; }
};















//template <typename Size>
//class FixNode {
//
//  protected:
//
//    Node* sons_[Size];
//    int last_so
//
//  public:
//
//    Node() : sons_() {}
//};


#endif // NODE_HPP

