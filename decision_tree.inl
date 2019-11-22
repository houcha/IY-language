#include <stack>
#include <string_view>


template <typename String>
DecisionTree::DecisionTree(const String& buffer)
    : DecisionTree() {
  ParseString(buffer);
}

template <typename String>
void DecisionTree::ParseString(const String& buffer) {
  std::stack<QueryNode*> unassigned_nodes;

  bool has_open_quote = false;
  // Points to open quote.
  const char* str_begin = nullptr;
  // Points to close quote.
  const char* str_end = nullptr;

  for (size_t i = 0; i < buffer.size(); ++i) {
    switch(buffer[i]) {
      case '{':
        {
          if (unassigned_nodes.empty()) {
            unassigned_nodes.push(root_);
          } else {
            QueryNode* new_node = new QueryNode();
            AddChild(unassigned_nodes.top(), new_node);
            unassigned_nodes.push(new_node);
          }
          break;
        }
      case '}':
        {
          unassigned_nodes.pop();
          break;
        }
      case '"':
        {
          if (has_open_quote) {
            has_open_quote = false;
            str_end = buffer.data() + i;
            (*unassigned_nodes.top()).str_ =
                std::string_view(str_begin + 1, str_end - str_begin - 1);
          } else {
            has_open_quote = true;
            str_begin = buffer.data() + i;
          }
          break;
        }
    }
  }
}

template <typename IteratorType>
IteratorType DecisionTree::Begin() {
  return BinaryTree<QueryNode>::Begin<IteratorType>();
}
template <typename IteratorType>
IteratorType DecisionTree::End() {
  return BinaryTree<QueryNode>::End<IteratorType>();
}

template <typename IteratorType>
IteratorType DecisionTree::Begin() const {
  return BinaryTree<QueryNode>::Begin<IteratorType>();
}
template <typename IteratorType>
IteratorType DecisionTree::End() const {
  return BinaryTree<QueryNode>::End<IteratorType>();
}

