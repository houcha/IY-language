#include "text.hpp"


int GetFileSize(FILE* file) {
  assert(file != nullptr);

  int start_pos = ftell(file);
  fseek(file, 0, SEEK_END);
  int file_size = ftell(file);
  fseek(file, start_pos, SEEK_SET);

  return file_size;
}

void Text::Load(FILE* src) {
  buffer_.resize(GetFileSize(src), 0);
  fread(buffer_.data(), sizeof(char), buffer_.size(), src);
}

