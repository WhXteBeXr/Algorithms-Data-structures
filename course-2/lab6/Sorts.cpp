#include "Sorts.h"

#include <utility>

void bubbleSort(std::span<int> values)
{
  for (std::size_t end = values.size(); end > 1; --end)
  {
    bool swapped = false;
    for (std::size_t i = 1; i < end; ++i)
    {
      if (values[i - 1] > values[i])
      {
        std::swap(values[i - 1], values[i]);
        swapped = true;
      }
    }
    // После прохода максимум уже в конце. Отсутствие обменов завершает сортировку.
    if (!swapped)
    {
      break;
    }
  }
}

void insertionSort(std::span<int> values)
{
  for (std::size_t i = 1; i < values.size(); ++i)
  {
    const int key = values[i];
    std::size_t position = i;
    // Левая часть уже отсортирована. Освобождаем место сдвигом больших значений.
    while (position > 0 && values[position - 1] > key)
    {
      values[position] = values[position - 1];
      --position;
    }
    values[position] = key;
  }
}

void selectionSort(std::span<int> values)
{
  for (std::size_t i = 0; i < values.size(); ++i)
  {
    // Префикс слева готов. В оставшейся части ищем значение для позиции i.
    std::size_t minimum = i;
    for (std::size_t j = i + 1; j < values.size(); ++j)
    {
      if (values[j] < values[minimum])
      {
        minimum = j;
      }
    }
    if (minimum != i)
    {
      std::swap(values[i], values[minimum]);
    }
  }
}
