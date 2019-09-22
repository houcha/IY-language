#include <cassert>
#include <string>
#include <string_view>
#include "cmp.h"
#include "sort.h"
#include "text.h"

/* ------------------------------------------------------------------
************************ MAJOR FUNCTIONS TESTS **********************
------------------------------------------------------------------ */

void TEST_FORWARD_CMP() {
  std::string_view a = "   ''''Hello-'''-world'''''  ";
  std::string_view b = "   ''''   hello world''''' ";
  std::string_view c = "   ''''   Hell1234world''''' ";

  auto cmp = ForwardCmp();

  assert(!cmp(a, b));
  assert(cmp(a, c));
  assert(cmp(b, c));
}

void TEST_reverse_CMP() {
  std::string_view a = "   ''''Hello-'''-world'''''  ";
  std::string_view b = "   ''''   hello world''''' ";
  std::string_view c = "   ''''He1234world''' ";

  auto cmp = ReverseCmp();

  assert(!cmp(b, a));
  assert(cmp(c, a));
  assert(cmp(c, b));
}

void TEST_LINES_FILLING() {
  std::vector<std::string> real_lines = {
      "     'He\\0llo world''", "     'Helloo312world'", "  asdfasdf    "};

  FILE* input_test_file = fopen("test_txt/input.txt", "r");
  Text poem(input_test_file);
  fclose(input_test_file);

  assert(real_lines.size() == poem.lines.size());

  for (int i = 0; i < real_lines.size(); ++i) {
    for (int j = 0; j < poem.lines[i].size(); ++j) {
      assert(poem.lines[i][j] == real_lines[i][j]);
    }
  }
}

void TEST_FORWARD_LINES_SORTING() {
  std::vector<std::string> real_forward_sorted_lines = {
      "  asdfasdf    ", "     'Helloo312world'", "     'He\\0llo world''"};

  FILE* input_test_file = fopen("test_txt/input.txt", "r");
  Text poem(input_test_file);
  fclose(input_test_file);

  SortLinesForward(poem.lines);

  assert(real_forward_sorted_lines.size() == poem.lines.size());

  for (int i = 0; i < poem.lines.size(); ++i) {
    for (int j = 0; j < poem.lines[i].size(); ++j) {
      assert(poem.lines[i][j] == real_forward_sorted_lines[i][j]);
    }
  }
}

void TEST_reverse_LINES_SORTING() {
  std::vector<std::string> real_reverse_sorted_lines = {
      "     'He\\0llo world''", "     'Helloo312world'", "  asdfasdf    "};

  FILE* input_test_file = fopen("test_txt/input.txt", "r");
  Text poem(input_test_file);
  fclose(input_test_file);

  SortLinesReverse(poem.lines);

  assert(real_reverse_sorted_lines.size() == poem.lines.size());

  for (int i = 0; i < poem.lines.size(); ++i) {
    for (int j = 0; j < poem.lines[i].size(); ++j) {
      assert(poem.lines[i][j] == real_reverse_sorted_lines[i][j]);
    }
  }
}

/* ------------------------------------------------------------------
************************ MINOR FUNCTIONS TESTS **********************
------------------------------------------------------------------ */

void TEST_CORRECT_SYMBOL() {
  assert(is_correct_symbol('i'));
  assert(!is_correct_symbol('8'));
  assert(!is_correct_symbol('`'));
  assert(!is_correct_symbol(' '));
  assert(is_correct_symbol('F'));
}

void TEST_SWITCH_TO_LOWER() {
  assert(switch_to_lower_case('i') == 'i');
  assert(switch_to_lower_case('Y') == 'y');
  assert(switch_to_lower_case('4') == '4');
}

int main() {
  TEST_CORRECT_SYMBOL();
  TEST_SWITCH_TO_LOWER();

  TEST_FORWARD_CMP();
  TEST_reverse_CMP();

  TEST_LINES_FILLING();

  TEST_FORWARD_LINES_SORTING();
  TEST_reverse_LINES_SORTING();

  return 0;
}
