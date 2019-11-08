template <typename String>
DecisionTree::DecisionTree(const String& buffer)
    : BinaryTree() {

  std::stack<QueryNode*> unassigned_nodes;
  unassigned_nodes.push(root_);

  size_t query_begin = 0;
  size_t query_end   = 0;

  size_t buffer_size = buffer.size();
  for (size_t i = 0; i < buffer_size; ++i) {
    if (buffer[i] == '(' || buffer[i] == ')') {
      query_end = i - 1;
      size_t query_length = query_end - query_begin + 1;
      if (query_length > 0) {
        (*unassigned_nodes.top()).query_ =
            std::string_view(buffer.data() + query_begin, query_length);
      }
      query_begin = i + 1;
      if (buffer[i] == '(') {
        QueryNode* new_node = new QueryNode();
        AddNode(unassigned_nodes.top(), new_node);
        unassigned_nodes.push(new_node);
      } else {   // buffer[i] == ')'
        unassigned_nodes.pop();
      }
    }
  }
}

void DecisionTree::AddNode(QueryNode* parent, QueryNode* son) {
  if (parent->left_ == nullptr) {
    parent->left_ = son;
  } else {
    parent->right_ = son;
  }
  ++count_nodes_;
}

