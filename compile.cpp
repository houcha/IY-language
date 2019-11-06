#include <cstring>  // strchr
#include "src/assembler.hpp"


int main(int argc, char* argv[]) {
  std::string noextension_filename(argv[1], strchr(argv[1], '.') - argv[1]);
  std::string assembled_filename(noextension_filename + ".prc.o");

  FILE* source    = fopen(argv[1], "r");
  FILE* assembled = fopen(assembled_filename.data(), "w+");

  Compile(source, assembled);

  fclose(source);
  fclose(assembled);
  return 0;
}

