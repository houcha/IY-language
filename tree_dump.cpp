#include "tree_dump.hpp"
#include "string_out.hpp"
#include <cstdio>


void DumpTree(const ExprTree& tree, FILE* file) {

  fprintf(file, "digraph {\n\tnode [shape=circle]\n");

  auto it = tree.Begin();
  ++it; // Skip root.
  while (it != tree.End()) {
    fprintf(file, "\tnode%p %.*s\n", (*it).GetParent(),
                                     UNFOLD((*it).GetParent()->GetGraphvizInfo()));
    fprintf(file, "\tnode%p %.*s\n", &(*it),
                                     UNFOLD((*it).GetGraphvizInfo()));
    fprintf(file, "\tnode%p -> node%p\n", (*it).GetParent(), &(*it));
    ++it;
  }
  fprintf(file, "}\n");
}

