/**
 * @file sort.h
 * @brief Header contains functions which sort poem strings.
 * @version 0.1
 * @date 2019-09-20
 *
 * @copyright Copyright (c) 2019
 *
 */

#include <vector>
#include <string_view>

/**
 * @brief Sort lines from the beginning.
 *
 * @param lines
 */
void SortLinesForward(std::vector<std::string_view>& lines);

/**
 * @brief Sort lines from the end.
 *
 * @param lines
 */
void SortLinesReverse(std::vector<std::string_view>& lines);

