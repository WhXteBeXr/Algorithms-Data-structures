#pragma once

#include <cstddef>
#include <set>
#include <string>
#include <string_view>
#include <vector>

struct LcsResult
{
  std::size_t length;
  std::set<std::string> sequences;
  std::vector<std::vector<std::size_t>> table;
};

// Все различные LCS в лексикографическом порядке. Вход - печатные символы ASCII.
// При длине 0 результат содержит одну пустую строку, а не пустое множество.
LcsResult findAllLcs(std::string_view first, std::string_view second);
