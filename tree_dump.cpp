#include "tree_dump.hpp"
#include <cstdio>


void DumpTree(const ExprTree& tree, FILE* file) {
  fprintf(file, "digraph {\n"
                "\tnode [shape=circle]\n");
  auto it = tree.Begin();
  ++it; // Skip root.
  while (it != tree.End()) {
    fprintf(file, "\t -> ");

    ++it;
  }
  fprintf(file, "}\n");
}

