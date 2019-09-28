#include "cmp.h"

bool is_correct_symbol(char str) {
  return (('a' <= str) && (str <= 'z')) || (('A' <= str) && (str <= 'Z'));
}

char switch_to_lower_case(char str) {
  if (('A' <= str) && (str <= 'Z')) {
    int upper_lower_char_diffeence = 'A' - 'a';
    return str - upper_lower_char_diffeence;
  }
  return str;
}

void move_to_next_correct_symbol(const std::string_view& str, int& cur_pos) {
  while (!is_correct_symbol(str[cur_pos]) && cur_pos < str.size()) ++cur_pos;
}

bool ForwardCmp::operator()(const std::string_view& a,
                            const std::string_view& b) {
  int i = 0, j = 0;

  while (i < a.size() && j < b.size()) {
    move_to_next_correct_symbol(a, i);
    move_to_next_correct_symbol(b, j);
    if (i < a.size() && j < b.size()) {
      if (switch_to_lower_case(a[i]) != switch_to_lower_case(b[j])) {
        char x = a[i];
        char y = b[j];
        return switch_to_lower_case(a[i]) < switch_to_lower_case(b[j]);
      } else {
        ++i;
        ++j;
      }
    }
  }
  if (i >= a.size() && j >= b.size()) {
    return false;
  }
  if (i >= a.size()) {
    return true;
  }
  return false;
}

void reverse_move_to_next_correct_symbol(const std::string_view& str,
                                         int& cur_pos) {
  while (!is_correct_symbol(str[cur_pos]) && cur_pos >= 0) {
    --cur_pos;
  }
}

bool ReverseCmp::operator()(const std::string_view& a,
                            const std::string_view& b) {
  int i = a.size() - 1, j = b.size() - 1;

  while (i >= 0 && j >= 0) {
    reverse_move_to_next_correct_symbol(a, i);
    reverse_move_to_next_correct_symbol(b, j);
    if (i >= 0 && j >= 0) {
      if (switch_to_lower_case(a[i]) != switch_to_lower_case(b[j])) {
        return switch_to_lower_case(a[i]) < switch_to_lower_case(b[j]);
      } else {
        --i;
        --j;
      }
    }
  }
  if (i < 0 && j < 0) {
    return false;
  }
  if (i < 0) {
    return true;
  }
  return false;
}

