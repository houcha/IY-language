#include <cstdio>
#include "text.hpp"
#include "expression_tree.hpp"
#include "tree_dump.hpp"


int main() {
  Text expression;
  FILE* expression_file = fopen("expression.txt", "r");
  expression.Load(expression_file);
  fclose(expression_file);

  ExprTree expr_tree(expression.Data());

  //ExprTree differentiated = expr_tree.Differentiate();

  FILE* dump_file = fopen("readen_tree.dot", "w");
  DumpTree(expr_tree, dump_file);
  fclose(dump_file);

  return 0;
}

