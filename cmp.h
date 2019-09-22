/**
 * @file cmp.h
 * @brief Header contains comparators which are used in poem sorting.
 * @version 0.1
 * @date 2019-09-20
 *
 * @copyright Copyright (c) 2019
 *
 */

#include <vector>
#include <string_view>

/**
 * @brief Check whether given symbol is a Latin letter.
 *
 * Used during letters comparsion in 'ForwardCmp' and 'BackwardCmp', since strings
 * capitalization does not count. (i.e. 'Y' and 'y' are equal)
 *
 * @param str
 * @return true
 * @return false
 */
bool is_correct_symbol(char str);

/**
 * @brief Return given letter in lower case.
 *
 * Used during letters comparsion in 'ForwardCmp' and 'BackwardCmp', since strings
 * capitalization does not count. (i.e. 'Y' and 'y' are equal)
 *
 * @param str
 * @return char
 */
char switch_to_lower_case(char str);

static inline
void move_to_next_correct_symbol(const std::string_view& str,
                                 int& cur_pos);

/**
 * @brief Comparator.
 *
 * Lexicographically compare strings from the beginning.
 * Count only Latin characters and pay no attention to CAPITALIZATION.
 * (i.e. strings "HELLO" and "hello" are equal)
 */
struct ForwardCmp {
  bool operator()(const std::string_view& a, const std::string_view& b);
};

static inline
void reverse_move_to_next_correct_symbol(const std::string_view& str,
                                         int& cur_pos);

/**
 * @brief Comparator.
 *
 * Lexicographically compare strings from the end.
 * Count only Latin characters and pay no attention to CAPITALIZATION.
 * (i.e. strings "HELLO" and "hello" are equal)
 */
struct ReverseCmp {
  bool operator()(const std::string_view& a, const std::string_view& b);
};

