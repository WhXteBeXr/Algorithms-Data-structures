#pragma once

#include <cstddef>
#include <string_view>
#include <vector>

// Все функции принимают печатный ASCII, регистр и пробелы учитываются.
std::vector<std::size_t> prefixFunction(std::string_view pattern);
// Все начальные позиции с нуля, включая перекрытия. Пустой образец даёт пустой список.
std::vector<std::size_t> kmpSearch(std::string_view text, std::string_view pattern);
// Сравнение наборов символов с учётом кратности через сортировку копий строк.
bool areAnagrams(std::string_view first, std::string_view second);
