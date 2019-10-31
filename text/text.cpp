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

void Text::SplitToLines() {
  FormatBufferAndCount();
  lines_.resize(lines_count_);
  FillLines();
}

static inline
void UpdateCount(int& cur_length, size_t& count) {
  if (cur_length != 0) {  // Do not count if empty.
    ++count;
    cur_length = 0;
  }
}

void Text::FormatBufferAndCount() {
  bool freeze = false;
  int cur_line_length = 0;
  int cur_word_length = 0;

  for (int pos = 0; pos < buffer_.size(); ++pos) {
    // If see comment, freeze counters until \n.
    if (buffer_[pos] == '#') {
      UpdateCount(cur_line_length, lines_count_);
      UpdateCount(cur_word_length, words_count_);
      freeze = true;
    } else
    if (buffer_[pos] == '\n') {
      freeze = false;
    }

    if (freeze) {
      buffer_[pos] = '\0';
    } else {
      // Count.
      if ((buffer_[pos] == ' ' || buffer_[pos] == '\n')) {
        UpdateCount(cur_word_length, words_count_);
      } else {
        ++cur_word_length;
      }
      if (buffer_[pos] == '\n') {
        UpdateCount(cur_line_length, lines_count_);
      } else {
        ++cur_line_length;
      }
      // Format.
      if (buffer_[pos] == '\n') {
        buffer_[pos] = '\0';
      }
    }
  }

  UpdateCount(cur_word_length, words_count_);
  UpdateCount(cur_line_length, lines_count_);
}

std::vector<std::string_view>
GetWords(const char* line, size_t line_size) {

  std::vector<std::string_view> words;

  const char* current    = line;
  const char* word_start = current;
  const char* data_end   = current + line_size;

  while (current != data_end) {
    // Seek for begin.
    while (current != data_end && (*current == ' ' || *current == '\t')) {
      ++current;
    }

    word_start = current;

    // Seek for end.
    while (current != data_end && (*current != ' ' && *current != '\t')) {
      ++current;
    }

    size_t length = current - word_start;
    if (length != 0) {
      words.emplace_back(std::string_view(word_start, length));
    }
  }

  return words;
}

void Text::FillLines() {
  int cur_line_num = 0;

  const char* current    = buffer_.data();
  const char* line_start = current;
  const char* data_end   = current + buffer_.size();

  while (current != data_end) {
    // Seek for begin.
    while (current != data_end && (*current == '\0')) {
      ++current;
    }

    line_start = current;

    // Seek for end.
    while (current != data_end && (*current != '\0')) {
      ++current;
    }

    size_t length = current - line_start;
    if (length != 0) {
      lines_[cur_line_num++] = GetWords(line_start, length);
    }
  }
}

