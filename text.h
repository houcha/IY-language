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
   * After Text has been constructed from file, its fields: `buffer` and `lines`
   * are correct.
   *
   * @param input_file
   */
  Text(FILE* input_file);
  ~Text() = default;

  /// Raw file content.
  std::string buffer;

  /// File text split onto lines. Do not contain empty strings.
  std::vector<std::string_view> lines;

 private:
  /// Input file pointer.
  FILE* file;

  /**
   * @function GetFileSize
   * @brief Get the file size.
   *
   * @param p_file
   * @return int
   */
  int get_file_size();

  /**
   * @brief Fill buffer with text from the file.
   *
   * @param p_data
   * @param lines_num
   * @param p_file
   */
  void fill_buffer();

  /**
   * @brief Construct lines from raw data.
   *
   * @param lines
   * @param p_data
   * @param data_size
   */
  void fill_lines();

  inline void update_lines_number(int& lines_number, int& cur_line_length);
  inline void seek_for_line_begin(const std::string& buffer, int& cur_pos);
  inline void seek_for_line_end  (const std::string& buffer, int& cur_pos);
};
