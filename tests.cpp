#include <iostream>
#include <string>
#include <string_view>
#include "cmp.h"
#include "sort.h"
#include "text.h"

#define ASSERT(state, error_message) {\
  if (!state) {\
    std::cout << __FILE__ << ", line: " << __LINE__ << "\nFalse statement: "\
    << #state << "\nError: " << error_message << '\n'; }\
}

/* ------------------------------------------------------------------
************************ MAJOR FUNCTIONS TESTS **********************
------------------------------------------------------------------ */

void TEST_FORWARD_CMP() {
  std::string_view a = "   ''''Hello-'''-world'''''  ";
  std::string_view b = "   ''''   hello world''''' ";
  std::string_view c = "   ''''   Hell1234world''''' ";

  auto cmp = ForwardCmp();

  ASSERT(!cmp(a, b), "bad forward cmp work");
  ASSERT(cmp(a, c), "bad forward cmp work");
  ASSERT(cmp(b, c), "bad forward cmp work");
}

void TEST_REVERSE_CMP() {
  std::string_view a = "   ''''Hello-'''-world'''''  ";
  std::string_view b = "   ''''   hello world''''' ";
  std::string_view c = "   ''''He1234world''' ";

  auto cmp = ReverseCmp();

  ASSERT(!cmp(b, a), "bad forward cmp work");
  ASSERT(cmp(c, a), "bad forward cmp work");
  ASSERT(cmp(c, b), "bad forward cmp work");
}

void TEST_LINES_FILLING() {
  std::vector<std::string> real_lines = {
      "     'He\\0llo world''", "     'Helloo312world'", "  asdfasdf    "};

  FILE* input_test_file = fopen("test_txt/input.txt", "r");
  Text poem(input_test_file);
  fclose(input_test_file);

  ASSERT(real_lines.size() == poem.lines.size(),
          "read lines number is different from specified");

  for (int i = 0; i < real_lines.size(); ++i) {
    for (int j = 0; j < poem.lines[i].size(); ++j) {
      ASSERT(poem.lines[i][j] == real_lines[i][j],
              "strings are not equal");
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

  ASSERT(real_forward_sorted_lines.size() == poem.lines.size(),
          "read lines number is different from specified");

  for (int i = 0; i < poem.lines.size(); ++i) {
    for (int j = 0; j < poem.lines[i].size(); ++j) {
      ASSERT(poem.lines[i][j] == real_forward_sorted_lines[i][j],
              "strings are not equal");
    }
  }
}

void TEST_REVERSE_LINES_SORTING() {
  std::vector<std::string> real_reverse_sorted_lines = {
      "     'He\\0llo world''", "     'Helloo312world'", "  asdfasdf    "};

  FILE* input_test_file = fopen("test_txt/input.txt", "r");
  Text poem(input_test_file);
  fclose(input_test_file);

  SortLinesReverse(poem.lines);

  ASSERT(real_reverse_sorted_lines.size() == poem.lines.size(),
          "read lines number is different from specified");

  for (int i = 0; i < poem.lines.size(); ++i) {
    for (int j = 0; j < poem.lines[i].size(); ++j) {
      ASSERT(poem.lines[i][j] == real_reverse_sorted_lines[i][j],
              "strings are not equal");
    }
  }
}

/* ------------------------------------------------------------------
************************ MINOR FUNCTIONS TESTS **********************
------------------------------------------------------------------ */

void TEST_CORRECT_SYMBOL() {
  ASSERT( is_correct_symbol('i'), "i is correct symbol");
  ASSERT(!is_correct_symbol('8'), "8 is incorrect symbol");
  ASSERT(!is_correct_symbol('`'), "` is incorrect symbol");
  ASSERT(!is_correct_symbol(' '), "  is incorrect symbol");
  ASSERT( is_correct_symbol('F'), "F is correct symbol");
}

void TEST_SWITCH_TO_LOWER_CASE() {
  ASSERT(switch_to_lower_case('i') == 'i', "bad switch to lower case work");
  ASSERT(switch_to_lower_case('Y') == 'y', "bad switch to lower case work");
  ASSERT(switch_to_lower_case('4') == '4', "bad switch to lower case work");
}

int main() {
  TEST_CORRECT_SYMBOL();
  TEST_SWITCH_TO_LOWER_CASE();

  TEST_FORWARD_CMP();
  TEST_REVERSE_CMP();

  TEST_LINES_FILLING();

  TEST_FORWARD_LINES_SORTING();
  TEST_REVERSE_LINES_SORTING();

  return 0;
}
