#include <iostream>
#include "text.hpp"
#include "decision_tree.hpp"
#include "node.hpp"


int main() {

  FILE* database_file = fopen("database.txt", "r");

  Text base;
  base.Load(database_file);

  DecisionTree tree(base.Data());

  PreOrderIterator<QueryNode> it = tree.Begin();
  PreOrderIterator<QueryNode> end_it = tree.End();
  for ( ; it != end_it; ++it) {
    std::cout << (*it).query_ << ' ' << (*it).left_->query_ << ' '
      << (*it).right_->query_ << '\n';
  }

  fclose(database_file);
  return 0;
}

