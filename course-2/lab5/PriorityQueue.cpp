#include "PriorityQueue.h"
#include "ArrayGrowth.h"

#include <stdexcept>
#include <utility>

void PriorityQueue::heapifyUp(std::size_t index)
{
  while (index > 0)
  {
    const std::size_t parent = (index - 1) / 2;
    if (heap[parent] >= heap[index])
    {
      break;
    }
    std::swap(heap[parent], heap[index]);
    index = parent;
  }
}

void PriorityQueue::heapifyDown(std::size_t index)
{
  while (index < count / 2) // Только у этих позиций существует левый ребёнок.
  {
    std::size_t child = index * 2 + 1;
    if (child + 1 < count && heap[child + 1] > heap[child])
    {
      ++child; // Выбираем большего ребёнка для восстановления max-heap.
    }
    if (heap[index] >= heap[child])
    {
      break;
    }
    std::swap(heap[index], heap[child]);
    index = child;
  }
}

void PriorityQueue::push(int value)
{
  lab5_detail::growArray(heap, capacity, count, count + 1);
  heap[count] = value;
  heapifyUp(count); // Новый элемент может нарушить порядок только к родителю.
  ++count;
}

int PriorityQueue::top() const
{
  if (isEmpty())
  {
    throw std::underflow_error("Приоритетная очередь пуста");
  }
  return heap[0];
}

int PriorityQueue::pop()
{
  const int result = top();
  --count;
  if (count != 0)
  {
    heap[0] = heap[count]; // Закрываем корень последним элементом и опускаем его.
    heapifyDown(0);
  }
  return result;
}
