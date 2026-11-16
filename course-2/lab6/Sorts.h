#pragma once

#include <span>

void bubbleSort(std::span<int> values);
void insertionSort(std::span<int> values);
void selectionSort(std::span<int> values);

// Одна таблица используется в демонстрации и замерах без дублирования списка функций.
struct SortAlgorithm
{
  const char* name;
  void (*sort)(std::span<int>);
};
inline constexpr SortAlgorithm sortingAlgorithms[] = {
    {"Bubble", bubbleSort}, {"Insertion", insertionSort}, {"Selection", selectionSort}};
