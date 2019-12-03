template <typename Node_t>
BinaryTree<Node_t>::~BinaryTree() {
  for (auto it = Begin<PreOrderIterator<Node_t>>();
            it != End<PreOrderIterator<Node_t>>(); ++it) {
    delete &(*it);
  }
}

