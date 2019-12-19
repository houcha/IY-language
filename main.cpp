#include "text.hpp"
#include "expression_parser.hpp"


int main() {
  Text program;
  FILE* program_file = fopen("program.txt", "r");
  program.Load(program_file);
  fclose(program_file);

  ExpressionParser parser(program.Data());
  printf("%d\n", parser.GetExpression());

  return 0;
}

