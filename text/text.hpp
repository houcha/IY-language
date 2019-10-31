#ifndef TEXT_HPP
#define TEXT_HPP

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
 * @brief Get the file size.
 *
 * @param file
 */
int GetFileSize(FILE* file);

/// Class which constructs from input file and ease work with its content.
class Text {

 protected:

  /// Raw file content.
  std::string buffer_;

  /// File text splitted onto lines (i.e. by \n character).
  /// Does not contain empty strings.
  std::vector<std::vector<std::string_view>> lines_;

  size_t words_count_;
  size_t lines_count_;

 public:

  /**
   * @brief Construct a new Text object with buffer filled with file content
   *
   * After Text has been constructed from file, its fields: `buffer_` and `lines_`
   * are correct.
   *
   * @param input_file
   */
  Text() : lines_      (),
           buffer_     (),
           lines_count_(0),
           words_count_(0) {}

  /// Loads file content to buffer.
  void Load(FILE* src);
  /// Parse buffer to lines.
  void SplitToLines();

  inline const std::string&
  Data()  const { return buffer_; }

  inline const std::vector<std::vector<std::string_view>>&
  Lines() const { return lines_; }

  size_t CountWords() const { return words_count_; }
  size_t CountLines() const { return lines_count_; }

 private:
  /// Replace \n with \0 in the buffer for next lines construction and count
  /// words and lines number.
  void FormatBufferAndCount();

  /// Construct lines from buffer.
  void FillLines();
};


#endif // TEXT_HPP

