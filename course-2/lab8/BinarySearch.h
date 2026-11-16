#pragma once

#include <cstddef>
#include <span>

// Требуется неубывающий массив. Возвращает первый индекс ключа или -1.
std::ptrdiff_t firstOccurrence(std::span<const int> values, int target);
