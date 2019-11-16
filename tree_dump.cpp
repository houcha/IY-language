#include "tree_dump.hpp"
#include <cstdio>


void DumpTree(const DecisionTree& tree, FILE* file) {
  fprintf(file, "digraph {\n\
\tnode [shape=box]\n");
  auto it = tree.Begin();
  ++it; // Skip root.
  while (it != tree.End()) {
    fprintf(file, "\t\"%.*s\" -> \"%.*s\" ",
        static_cast<int>((*it).parent_->str_.size()),
                         (*it).parent_->str_.data(),
        static_cast<int>((*it).str_.size()),
                         (*it).str_.data());
    if ((*it).IsRightChild()) {
      fprintf(file, "[label=\"да\"];\n");
    } else {
      fprintf(file, "[label=\"нет\"];\n");
    }
    ++it;
  }
  fprintf(file, "}\n");
}

