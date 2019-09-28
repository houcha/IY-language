#include "text.h"

Text::Text(FILE* input_file) :
  lines_(),
  buffer_() {

    buffer_.resize(get_file_size(input_file));
    fill_buffer(input_file);
    int lines_count = 0;
    format_buffer_and_count_lines(lines_count);
    lines_.resize(lines_count);
    fill_lines();
}

int Text::get_file_size(FILE* file) {
  assert(file != nullptr);

  int start_pos = ftell(file);
  fseek(file, 0, SEEK_END);
  int file_size = ftell(file);
  fseek(file, start_pos, SEEK_SET);

  return file_size;
}

void Text::fill_buffer(FILE* file) {
  fread(buffer_.data(), sizeof(char), buffer_.size(), file);
}

inline void update_lines_number(int& lines_number, int& cur_line_length) {
  if (cur_line_length != 0) {  // Do not count empty lines.
    ++lines_number;
    cur_line_length = 0;
  }
}

void Text::format_buffer_and_count_lines(int& lines_number) {
  int cur_line_length = 0;

  for (int pos = 0; pos < buffer_.size(); ++pos) {
    if (buffer_[pos] == '\n') {
      buffer_[pos] = '\0';
      update_lines_number(lines_number, cur_line_length);
    } else {  // Do not count \n as a symbol.
      ++cur_line_length;
    }
  }
  update_lines_number(lines_number, cur_line_length);
}

inline void seek_for_line_begin(const std::string& buffer, int& cur_pos) {
  while (buffer[cur_pos] == '\0' && cur_pos < buffer.size()) {
    ++cur_pos;
  }
}

inline void seek_for_line_end(const std::string& buffer, int& cur_pos) {
  while (buffer[cur_pos] != '\0') {
    cur_pos++;
  }
}

void Text::fill_lines() {
  int cur_line_num = 0, cur_pos = 0;

  while (cur_pos < buffer_.size()) {
    seek_for_line_begin(buffer_, cur_pos);
    if (cur_pos >= buffer_.size()) {
      return;
    }
    lines_[cur_line_num++] = std::string_view(buffer_.data() + cur_pos);
    seek_for_line_end(buffer_, cur_pos);
  }
}

