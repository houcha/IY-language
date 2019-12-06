#include <cstdio>
#include "text.hpp"
#include "expression_tree.hpp"

void WritePreamble(FILE* texfile) {
  fprintf(texfile,
      "\\documentclass{article}\n"
      "\\usepackage{amsmath}\n"
      "\\title{Differentiation}\n"
      "\\begin{document}\n"
      "  \\maketitle\n");
}

void WriteEpilogue(FILE* texfile) {
  fprintf(texfile, "\\end{document}");
}

int main() {
  Text expression;
  FILE* expression_file = fopen("expression.txt", "r");
  expression.Load(expression_file);
  fclose(expression_file);

  ExprTree expr_tree(expression.Data());

  FILE* read_file = fopen("readen_tree.dot", "w");
  expr_tree.Dump(read_file);
  fclose(read_file);

  FILE* solution = fopen("differentiation.tex", "w");
  WritePreamble(solution);
  ExprTree differentiated = expr_tree.Differentiate("x", solution);
  WriteEpilogue(solution);
  fclose(solution);

  FILE* diff_file = fopen("differentiated_tree.dot", "w");
  differentiated.Dump(diff_file);
  fclose(diff_file);

  return 0;
}

