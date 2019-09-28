/**
 * @file output.h
 * @brief Header contains functions which print different texts into files.
 * @version 0.1
 * @date 2019-09-20
 *
 * @copyright Copyright (c) 2019
 *
 */

#include <string>
#include <string_view>
#include <vector>

/**
 * @brief Write lines separated with `\n` into output file.
 *
 * @param lines
 * @param lines_num
 * @param p_out_file
 */
void OutputLines(const std::vector<std::string_view>& lines, FILE* output_file);

/**
 * @brief Write virgin (initial) text into output file.
 *
 * @param p_data
 * @param data_size
 * @param p_out_file
 */
void OutputText(const std::string& text, FILE* output_file);

