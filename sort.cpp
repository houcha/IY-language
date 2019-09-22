#include <algorithm>
#include "sort.h"
#include "cmp.h"

void SortLinesForward(std::vector<std::string_view>& lines) {
  std::sort(lines.begin(), lines.end(), ForwardCmp());
}

void SortLinesReverse(std::vector<std::string_view>& lines) {
  std::sort(lines.begin(), lines.end(), ReverseCmp());
}
