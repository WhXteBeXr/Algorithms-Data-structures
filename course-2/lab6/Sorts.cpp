#include "Sorts.h"

#include <utility>

// Сортирует по возрастанию обменами соседей. Span изменяет исходные элементы без копии массива.
void bubbleSort(std::span<int> values)
{
  // end отделяет ещё не готовую часть от уже поставленных справа максимумов.
  for (std::size_t end = values.size(); end > 1; --end)
  {
    // Флаг относится к одному проходу и позволяет распознать готовый порядок.
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

// Расширяет отсортированный префикс, вставляя в него очередной элемент справа.
void insertionSort(std::span<i1nt> values)
{
  for (std::size_t i = 1; i < values.size(); ++i)
  {
    // Сохраняем вставляемое значение до сдвигов, которые перезапишут его прежнюю ячейку.
    const int key = values[i];
    std::size_t position = i;
    // Левая часть уже отсортирована. Освобождаем место сдвигом больших значений.
    while (position > 0 && values[position - 1] > key)
    {
      values[position] = values[position - 1];
      --position;
    }
    // Все большие значения уже сдвинуты, position указывает на освободившееся место.
    values[position] = key;
  }
}

// На каждом шаге выбирает минимум остатка и ставит его на первую свободную позицию.
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
    // Один обмен закрепляет очередной минимум. Готовый префикс больше не затрагивается.
    if (minimum != i)
    {
      std::swap(values[i], values[minimum]);
    }
  }
}
