/**
 * \mainpage Poem sorting problem.
 *
 * In this project you can find my solution of poem sorting problem.
 *
 * The task is to read Latin poem from `input.txt` file and then:
 * 1. Writes lines sorted on Latin characters from the beginning into `forward.txt`.
 * 2. Writes lines sorted on Latin characters from the end into `reverse.txt`.
 * 3. Writes virgin (initial) poem text into `origin.txt`.
 *
 * All files are located in `txt` subdirectory of the project.
 *
 */

/**
 * @file main.cpp
 * @version 0.1
 * @date 2019-09-20
 *
 *
 * @copyright Copyright (c) 2019
 *
 */

#include "output.h"
#include "sort.h"
#include "text.h"

int main() {
  FILE* input_file = fopen("txt/input.txt", "r");
  assert(input_file);
  FILE* output_forward_file = fopen("txt/forward.txt", "w");
  assert(output_forward_file);
  FILE* output_reverse_file = fopen("txt/reverse.txt", "w");
  assert(output_reverse_file);
  FILE* output_origin_file = fopen("txt/origin.txt", "w");
  assert(output_origin_file);

  Text poem(input_file);

  SortLinesForward(poem.lines);
  OutputLines(poem.lines, output_forward_file);

  SortLinesReverse(poem.lines);
  OutputLines(poem.lines, output_reverse_file);

  OutputText(poem.buffer, output_origin_file);

  fclose(input_file);
  fclose(output_forward_file);
  fclose(output_reverse_file);
  fclose(output_origin_file);

  return 0;
}
