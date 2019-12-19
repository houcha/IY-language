#include "text.hpp"
#include "grammar_parser.hpp"


int main() {
  Text program;
  FILE* program_file = fopen("program.txt", "r");
  program.Load(program_file);
  fclose(program_file);

  GrammarParser parser(program.Data());
  printf("%d\n", parser.GetValue());

  return 0;
}

