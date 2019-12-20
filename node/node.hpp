#ifndef NODE_HPP
#define NODE_HPP

#include <cstdio>
#include <vector>
#include <string>


class Node {

  protected:

    //std::vector<Node*> sons_;
    Node* left_;
    Node* right_;
    Node* parent_;

  public:

    Node() : left_  (nullptr),
             right_ (nullptr),
             parent_(nullptr) {}

    Node(Node* left, Node* right) : left_  (left),
                                    right_ (right),
                                    parent_(nullptr) {}
    virtual ~Node();

    bool IsLeaf()       const;
    bool IsRoot()       const;
    bool IsLeftChild()  const;
    bool IsRightChild() const;

    void AddChild(Node* child);
    //void DelChild(Node* child);

    void ReplaceLeft (Node* child);
    void ReplaceRight(Node* child);

    Node* GetLeft()   const;
    Node* GetRight()  const;
    Node* GetParent() const;
    /// Return string which describes node as GraphViz element.
    void WriteToGraphviz(FILE* graphfile) const;

    virtual const std::string GetString() const = 0;

  protected:

    virtual const char* GetColor() const;
    virtual void Update() {};

  private:

    void WriteThisToGraphviz(FILE* graphfile) const;
};


class FunctionDeclNode : public Node {

  private:

    // left_ is args.
    // right_ is body.
    std::string name_;

  public:

    FunctionDeclNode() {}

    virtual const std::string GetString() const { return name_; }
};


class FunctionCallNode : public Node {

  private:

    // left_ is args.
    // right_ is body.
    std::string name_;

  public:

    FunctionCallNode(const std::string& name, Node* args)
        : name_(name),
          Node(args, nullptr) {}

    virtual const std::string GetString() const { return name_; }
};


class ArgsNode : public Node {

  private:

    // left_ is args.
    std::vector<Node*> args_;

  public:

    ArgsNode() : args_() {}

    void AddArg(Node* node) {}

    virtual const std::string GetString() const { return "ARGS"; }
};


class IdentifierNode : public Node {

  private:

    std::string name_;

  public:

    IdentifierNode(const std::string& name)
        : name_(name) {}

    virtual const std::string GetString() const override { return name_; }
};


class AssignNode : public Node {

  public:

    AssignNode(Node* var, Node* expr)
        : Node(var, expr) {}

    virtual const std::string GetString() const override { return "="; }
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

