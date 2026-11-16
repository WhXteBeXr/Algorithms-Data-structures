#include "BinarySearch.h"

std::ptrdiff_t firstOccurrence(std::span<const int> values, int target)
{
  std::size_t left = 0;
  std::size_t right = values.size(); // Полуоткрытая граница безопасна и для пустого массива.
  while (left < right)
  {
    const std::size_t middle = left + (right - left) / 2;
    if (values[middle] < target)
    {
      left = middle + 1;
    }
    else
    {
      right = middle; // При равенстве продолжаем влево, не теряя первое вхождение.
    }
  }
  // Найдена первая позиция со значением >= target, но равенство ещё не гарантировано.
  if (left < values.size() && values[left] == target)
  {
    return static_cast<std::ptrdiff_t>(left);
  }
  return -1;
}
