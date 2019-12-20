#include "text.hpp"
#include "expression_parser.hpp"


int main() {
  Text program;
  FILE* program_file = fopen("program.txt", "r");
  program.Load(program_file);
  fclose(program_file);

  Parser parser(program.Data());
  Node* tree = parser.GetExpression();

  FILE* tree_file = fopen("tree.dot", "w");
  fprintf(tree_file, "digraph {\n\tnode [shape=circle]\n");
  tree->WriteToGraphviz(tree_file);
  fprintf(tree_file, "}");
  fclose(tree_file);

  return 0;
}

