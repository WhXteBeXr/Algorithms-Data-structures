#pragma once

#include <span>

// Обе функции изменяют переданный диапазон int, не выделяя отдельный массив.
void buildMinHeap(std::span<int> values);
void heapSortDescending(std::span<int> values);
