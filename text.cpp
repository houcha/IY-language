#include "text.h"

Text::Text(FILE* input_file) : file(input_file), lines(), buffer() {
  // +1 because of extra \0 at the end of data that will be added leter on.
  // '\0' is to be, for std::string_view constructor expects terminated-null
  // as a signal of raw data end.
  buffer.resize(get_file_size() + 1);
  fill_buffer();
  fill_lines();
}

int Text::get_file_size() {
  assert(file != nullptr);

  fseek(file, 0, SEEK_END);
  int file_size = ftell(file);
  fseek(file, 0, SEEK_SET);

  return file_size;
}

void Text::update_lines_number(int& lines_number, int& cur_line_length) {
  if (cur_line_length != 0) {  // Do not count empty lines.
    ++lines_number;
    cur_line_length = 0;
  }
}

void Text::fill_buffer() {
  int cur_line_length = 0, pos = 0;
  int lines_number = 0;

  for (char symbol; (symbol = fgetc(file)) != EOF; ++pos) {
    if (symbol == '\n') {
      symbol = '\0';
      update_lines_number(lines_number, cur_line_length);
    } else {  // Do not count \n as a symbol.
      ++cur_line_length;
    }
    buffer[pos] = symbol;
  }
  update_lines_number(lines_number, cur_line_length);
  buffer[pos] = '\0';  // Last symbol is an extra '\0'.

  lines.resize(lines_number);
}

void Text::fill_lines() {
  int cur_line_num = 0, cur_pos = 0;

  while (cur_pos < buffer.size()) {
    seek_for_line_begin(buffer, cur_pos);
    if (cur_pos >= buffer.size()) {
      return;
    }
    lines[cur_line_num++] = std::string_view(buffer.data() + cur_pos);
    seek_for_line_end(buffer, cur_pos);
  }
}

void Text::seek_for_line_begin(const std::string& buffer, int& cur_pos) {
  while (buffer[cur_pos] == '\0' && cur_pos < buffer.size()) {
    ++cur_pos;
  }
}

void Text::seek_for_line_end(const std::string& buffer, int& cur_pos) {
  while (buffer[cur_pos] != '\0') {
    cur_pos++;
  }
}
