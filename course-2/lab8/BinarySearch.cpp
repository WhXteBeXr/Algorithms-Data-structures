#include "BinarySearch.h"

// Ищет первое вхождение target в массиве, заранее отсортированном по возрастанию.
// Возвращает индекс или -1 при отсутствии, исходные элементы не меняет. Время O(log N).
std::ptrdiff_t firstOccurrence(std::span<const int> values, int target)
{
  std::size_t left = 0;
  std::size_t right = values.size(); // Полуоткрытая граница безопасна и для пустого массива.
  while (left < right)
  {
    // Такая запись середины не складывает два потенциально больших индекса.
    const std::size_t middle = left + (right - left) / 2;
    if (values[middle] < target)
    {
      // Всё до middle включительно меньше target и больше не может содержать ответ.
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
  // Знаковый тип результата позволяет отличить промах от любого допустимого индекса.
  return -1;
}
