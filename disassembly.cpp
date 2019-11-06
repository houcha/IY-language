#include <cstring>  // strchr
#include "src/disassembler.hpp"

#include <iostream>

int main(int argc, char* argv[]) {
  //std::string noextension_filename(argv[1], strchr(argv[1], '.') - argv[1]);
  //std::string disassembled_filename(noextension_filename + ".dis.asm");
//
//  FILE* assembled    = fopen(argv[1], "r");
//  FILE* disassembled = fopen(disassembled_filename.data(), "w+");
  FILE* assembled    = fopen("asm/square_equation.prc.o", "r");
  FILE* disassembled = fopen("asm/square_equation.dis.asm", "w+");

  Disassembly(assembled, disassembled);

  fclose(assembled);
  fclose(disassembled);
  return 0;
}

