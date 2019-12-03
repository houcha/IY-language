#ifndef BIN_TREE_ITER_HPP
#define BIN_TREE_ITER_HPP

#include <iterator>
#include <stack>


template <typename Node_t>
class BinaryTreeIterator {

  public:

    typedef Node_t                    value_type;
    typedef value_type&               reference;
    typedef value_type*               pointer;
    typedef size_t                    difference_type;
    typedef std::forward_iterator_tag iterator_category;

  protected:

    pointer current_;

  public:

    BinaryTreeIterator(pointer root)
        : current_(root) {}

    virtual BinaryTreeIterator& operator++() = 0;

    reference operator*() {
      return *current_;
    }
    bool operator==(const BinaryTreeIterator& rhs) {
      return current_ == rhs.current_;
    }
    bool operator!=(const BinaryTreeIterator& rhs) {
      return current_ != rhs.current_;
    }
};


template <typename Node_t>
class DFSIterator : public BinaryTreeIterator<Node_t> {

  public:

    typedef
      typename BinaryTreeIterator<Node_t>::value_type        value_type;
    typedef
      typename BinaryTreeIterator<Node_t>::reference         reference;
    typedef
      typename BinaryTreeIterator<Node_t>::pointer           pointer;
    typedef
      typename BinaryTreeIterator<Node_t>::difference_type   difference_type;
    typedef
      typename BinaryTreeIterator<Node_t>::iterator_category iterator_category;

  protected:

    std::stack<pointer> dfs_;

  public:

    DFSIterator(pointer root)
        : BinaryTreeIterator<Node_t>(root),
          dfs_() {}
};


template <typename Node_t>
class PreOrderIterator : public DFSIterator<Node_t> {

  public:

    typedef
      typename DFSIterator<Node_t>::value_type        value_type;
    typedef
      typename DFSIterator<Node_t>::reference         reference;
    typedef
      typename DFSIterator<Node_t>::pointer           pointer;
    typedef
      typename DFSIterator<Node_t>::difference_type   difference_type;
    typedef
      typename DFSIterator<Node_t>::iterator_category iterator_category;

    PreOrderIterator(pointer root)
        : DFSIterator<Node_t>(root) {
      PushChildren(root);
    }

    // In-place because templates makes outer implementation too big.
    BinaryTreeIterator<Node_t>& operator++() override {
      if (this->dfs_.empty()) {
        this->current_ = nullptr;
      } else {
        this->current_ = this->dfs_.top(); this->dfs_.pop();
        PushChildren(this->current_);
      }
      return *this;
    }

  private:
    void PushChildren(pointer node) {
      if (node == nullptr) return;
      if (node->GetRight() != nullptr) {
        this->dfs_.push(node->GetRight());
      }
      if (node->GetLeft()  != nullptr) {
        this->dfs_.push(node->GetLeft());
      }
    }
};


#endif // BIN_TREE_ITER_HPP

