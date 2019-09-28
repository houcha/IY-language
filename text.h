/**
 * @file text.h
 * @brief Header contains Text class.
 * @version 0.1
 * @date 2019-09-20
 *
 * @copyright Copyright (c) 2019
 *
 */

#include <cassert>
#include <string>
#include <string_view>
#include <vector>

/**
 * @brief Class which constructs from input file and ease work with its content.
 */
class Text {
 public:
  /**
   * @brief Construct a new Text object with buffer filled with file content
   *
   * After Text has been constructed from file, its fields: `buffer_` and `lines_`
   * are correct.
   *
   * @param input_file
   */
  explicit Text(FILE* input_file);
  ~Text() = default;

  /// Raw file content.
  std::string buffer_;

  /// File text split onto lines. Do not contain empty strings.
  std::vector<std::string_view> lines_;

 private:
  /**
   * @brief Get the file size.
   *
   * @param file
   */
  int get_file_size(FILE* file);

  /**
   * @brief Fill buffer with text from the file.
   *
   * @param file
   */
  void fill_buffer(FILE* file);

  /**
   * @brief Replace \n with \0 in the buffer for next lines construction.
   *
   * @param lines_number
   */
  void format_buffer_and_count_lines(int& lines_number);

  /**
   * @brief Construct lines from buffer.
   */
  void fill_lines();
};

