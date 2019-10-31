#include <iostream>
#include "assembler.hpp"
#include "processor.hpp"

int main() {

  FILE* input = fopen("factorial.asm", "r");
  FILE* assembled = fopen("assembly.asm", "w+");
  FILE* disassembled = fopen("disassemblied.txt", "w");

  Compile(input, assembled);
  rewind(assembled);
  Disassembly(assembled, disassembled);

  //Run(assembled);

  return 0;
}

