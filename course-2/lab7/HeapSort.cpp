#include "HeapSort.h"

#include <utility>

namespace
{
  // Восстанавливает min-heap вниз от root, если поддеревья его детей уже являются кучами.
  // Изменяет элементы переданного диапазона, дополнительных узлов не создаёт.
  void siftDown(std::span<int> heap, std::size_t root)
  {
    // У позиций начиная с size/2 нет детей. Их порядок уже корректен.
    while (root < heap.size() / 2)
    {
      // В массивном дереве левый ребёнок имеет индекс 2*root+1, правый следует за ним.
      std::size_t child = root * 2 + 1;
      if (child + 1 < heap.size() && heap[child + 1] < heap[child])
      {
        ++child; // Для min-heap выбираем меньшего из существующих детей.
      }
      // Если родитель не больше меньшего ребёнка, порядок соблюдён для обоих детей.
      if (heap[root] <= heap[child])
      {
        break;
      }
      std::swap(heap[root], heap[child]);
      root = child; // Нарушение могло остаться только в поддереве этого ребёнка.
    }
  }
}

// Преобразует весь диапазон в min-heap за O(N), минимум окажется на позиции 0.
void buildMinHeap(std::span<int> values)
{
  // Идём снизу вверх: к обработке родителя поддеревья его детей уже являются кучами.
  // Форма цикла не вычисляет size/2 - 1 для пустого массива.
  for (std::size_t parent = values.size() / 2; parent > 0; --parent)
  {
    siftDown(values, parent - 1);
  }
}

// Сортирует исходный диапазон по убыванию за O(N log N), используя O(1) дополнительной памяти.
void heapSortDescending(std::span<int> values)
{
  // После построения кучи корень содержит минимум ещё не размещённых элементов.
  buildMinHeap(values);
  // end - граница после рабочей кучи. Для 0 или 1 элемента извлекать минимум не нужно.
  for (std::size_t end = values.size(); end > 1; --end)
  {
    // Минимум переносится в конец рабочей части. Готовый суффикс растёт справа налево.
    std::swap(values[0], values[end - 1]);
    // Готовые элементы исключены из диапазона и больше не участвуют в просеивании.
    siftDown(values.first(end - 1), 0);
  }
}
