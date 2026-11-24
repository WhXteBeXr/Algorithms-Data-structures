#include "Knapsack.h"

#include <algorithm>
#include <limits>
#include <stdexcept>

// Решает рюкзак 0/1: каждый предмет можно взять один раз, общий вес не превышает capacity.
// Возвращает максимальную ценность, индексы выбранных предметов и таблицу состояний.
KnapsackResult solveKnapsack(std::span<const Item> items, int capacity)
{
  // Проверяем знаки до преобразования в беззнаковые индексы таблицы.
  if (capacity < 0)
  {
    throw std::invalid_argument("Вместимость не может быть отрицательной");
  }
  for (const auto& item : items)
  {
    if (item.weight < 0)
    {
      throw std::invalid_argument("Вес предмета не может быть отрицательным");
    }
  }
  // table[i][w] - лучшая ценность среди первых i предметов при вместимости w.
  // Нулевая строка означает отсутствие предметов и задаёт основу переходов.
  const auto columns = static_cast<std::size_t>(capacity) + 1;
  KnapsackResult result{0, {}, std::vector<std::vector<std::int64_t>>(
    items.size() + 1, std::vector<std::int64_t>(columns, 0))};
  auto& table = result.table;
  for (std::size_t i = 1; i <= items.size(); ++i)
  {
    const auto weight = static_cast<std::size_t>(items[i - 1].weight);
    const auto value = items[i - 1].value;
    // Столбец 0 тоже вычисляется: предметы нулевого веса могут иметь ценность.
    for (std::size_t w = 0; w < columns; ++w)
    {
      table[i][w] = table[i - 1][w]; // Сначала вариант без нового предмета.
      if (weight <= w)
      {
        // Если берём предмет, оставшуюся вместимость заполняем только предыдущими предметами.
        const auto previous = table[i - 1][w - weight];
        // Ценности состояний неотрицательны, поэтому опасно только сложение с положительным value.
        if (value > 0 && previous > std::numeric_limits<std::int64_t>::max() - value)
        {
          throw std::overflow_error("Суммарная ценность не помещается в int64_t");
        }
        // Обе альтернативы читают предыдущую строку, предмет не используется повторно.
        table[i][w] = std::max(table[i][w], previous + value);
      }
    }
  }
  // Правый нижний элемент использует все предметы и всю доступную вместимость.
  result.maxValue = table.back().back();
  // Идём назад по строкам: отличие от предыдущей строки означает выбор текущего предмета.
  auto remaining = static_cast<std::size_t>(capacity);
  for (std::size_t i = items.size(); i > 0; --i)
  {
    if (table[i][remaining] != table[i - 1][remaining])
    {
      result.selected.push_back(i - 1);
      // Для следующего шага восстановления оставляем вместимость до добавления этого предмета.
      remaining -= static_cast<std::size_t>(items[i - 1].weight);
    }
    // Даже при remaining == 0 продолжаем: могли быть выбраны предметы нулевого веса.
  }
  // Обратный проход собрал индексы с конца, возвращаем их в исходном порядке предметов.
  std::reverse(result.selected.begin(), result.selected.end());
  return result;
}
