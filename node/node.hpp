#ifndef NODE_HPP
#define NODE_HPP


class Node {

  protected:

    Node* left_;
    Node* right_;
    Node* parent_;

  public:

    Node() : left_  (nullptr),
             right_ (nullptr),
             parent_(nullptr) {}

    bool IsLeaf()       const;
    bool IsRoot()       const;
    bool IsLeftChild()  const;
    bool IsRightChild() const;

    void AddChild(Node* child);
    void ReplaceLeft(Node* child);
    void ReplaceRight(Node* child);

    virtual void Update() {};
};


#endif // NODE_HPP

