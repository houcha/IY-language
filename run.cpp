#include <cstring>              // strchr, strcmp
#include "src/processor.hpp"


int main(int argc, char* argv[]) {
  std::string noextension_filename(argv[1], strchr(argv[1], '.') - argv[1]);

  FILE* assembled = fopen(argv[1], "r");
  FILE* listing   = nullptr;

  for (size_t i = 2; i < argc; ++i) {
    if (strcmp(argv[i], "--make-listing") == 0) {
      std::string listing_filename(noextension_filename + ".lst");
      listing = fopen(listing_filename.data(), "w+");
    }
  }

  Run(assembled, listing);

  fclose(assembled);
  if (listing != nullptr) {
    fclose(listing);
  }
  return 0;
}

