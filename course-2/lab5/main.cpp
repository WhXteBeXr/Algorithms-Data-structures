#include "Deque.h"
#include "PriorityQueue.h"
#include "Queue.h"
#include "Stack.h"
#include "Tasks.h"

#include <exception>
#include <iostream>
#include <limits>

namespace
{
  template <typename Action>
  void showError(const char* label, Action action)
  {
    try
    {
      action();
    }
    catch (const std::exception& error)
    {
      std::cout << label << ": " << error.what() << '\n';
    }
  }

  void demonstrateStack()
  {
    std::cout << "=== Стек и двоичная запись ===\n";
    Stack stack;
    stack.push(10);
    stack.push(20);
    std::cout << "size = " << stack.size() << ", top = " << stack.top()
              << ", pop = " << stack.pop() << ", затем top = " << stack.top() << '\n';
    // Ноль, знак и границы int64_t показывают, что перевод не переполняется.
    const std::int64_t numbers[] = {0, 13, -13, std::numeric_limits<std::int64_t>::min(),
                                   std::numeric_limits<std::int64_t>::max()};
    for (std::int64_t number : numbers)
    {
      std::cout << number << " -> " << toBinary(number) << '\n';
    }
  }

  void demonstrateQueue()
  {
    std::cout << "\n=== Очередь и максимум за O(1) ===\n";
    Queue queue;
    for (int value : {5, 1, 5, 3})
    {
      queue.enqueue(value);
    }
    std::cout << "front = " << queue.front() << ", size = " << queue.size() << ", max = " << queue.getMax() << '\n';
    // Повторяющийся максимум должен сохраниться после удаления первого 5.
    while (!queue.isEmpty())
    {
      std::cout << "dequeue = " << queue.dequeue();
      if (!queue.isEmpty())
      {
        std::cout << ", max = " << queue.getMax();
      }
      std::cout << '\n';
    }
    // Максимум отрицательных чисел нельзя ошибочно заменять нулём.
    queue.enqueue(-9);
    queue.enqueue(-2);
    std::cout << "После повторного заполнения: front = " << queue.front() << ", max = " << queue.getMax() << '\n';
  }

  void demonstrateDeque()
  {
    std::cout << "\n=== Дек и максимум за O(1) ===\n";
    Deque deque;
    for (int value : {-4, 5, 3, 5, 2, 8})
    {
      deque.push_back(value);
    }
    std::cout << "front = " << deque.front() << ", back = " << deque.back() << ", max = " << deque.getMax() << '\n';
    // Первое извлечение слева перераспределяет элементы, добавленные справа.
    std::cout << "pop_front = " << deque.pop_front() << ", pop_back = " << deque.pop_back()
              << ", max = " << deque.getMax() << '\n';
    deque.push_front(9);
    deque.push_back(9);
    std::cout << "Удаляем две девятки: " << deque.pop_front() << ", max = " << deque.getMax();
    std::cout << ", затем " << deque.pop_back() << ", max = " << deque.getMax() << '\n';
    bool fromFront = true;
    std::cout << "Чередование концов:";
    while (!deque.isEmpty())
    {
      std::cout << ' ' << (fromFront ? deque.pop_front() : deque.pop_back());
      fromFront = !fromFront;
    }
    std::cout << ", пуст = " << deque.isEmpty() << '\n';
    // Теперь проверяем перераспределение в противоположную сторону.
    for (int value = 1; value <= 8; ++value)
    {
      deque.push_front(value);
    }
    std::cout << "После вставок слева: pop_back = " << deque.pop_back() << ", front = " << deque.front()
              << ", size = " << deque.size() << ", max = " << deque.getMax() << '\n';
  }

  void demonstrateHeap()
  {
    std::cout << "\n=== Приоритетная очередь и Heap Sort ===\n";
    PriorityQueue queue;
    for (int value : {3, -1, 7, 7, 0})
    {
      queue.push(value);
    }
    std::cout << "top = " << queue.top() << ", size = " << queue.size() << "\nИзвлечение:";
    while (!queue.isEmpty())
    {
      std::cout << ' ' << queue.pop();
    }
    int values[] = {9, -4, 3, 9, 0, -1};
    heapSort(values);
    std::cout << "\nСортировка [9, -4, 3, 9, 0, -1]:";
    for (int value : values)
    {
      std::cout << ' ' << value;
    }
    int single[] = {42};
    heapSort(single);
    heapSort({}); // Пустой диапазон не вызывает pop.
    std::cout << "\nОдин элемент: " << single[0] << ", пустой диапазон обработан\n";
  }

  void demonstrateErrors()
  {
    std::cout << "\n=== Операции с пустыми структурами ===\n";
    Stack stack;
    Queue queue;
    Deque deque;
    PriorityQueue heap;
    showError("Stack::pop", [&] { stack.pop(); });
    showError("Stack::top", [&] { stack.top(); });
    showError("Stack::getMax", [&] { stack.getMax(); });
    showError("Queue::dequeue", [&] { queue.dequeue(); });
    showError("Queue::front", [&] { queue.front(); });
    showError("Queue::getMax", [&] { queue.getMax(); });
    showError("Deque::pop_front", [&] { deque.pop_front(); });
    showError("Deque::pop_back", [&] { deque.pop_back(); });
    showError("Deque::front", [&] { deque.front(); });
    showError("Deque::back", [&] { deque.back(); });
    showError("Deque::getMax", [&] { deque.getMax(); });
    showError("PriorityQueue::pop", [&] { heap.pop(); });
    showError("PriorityQueue::top", [&] { heap.top(); });
  }
}

int main()
{
  std::cout << std::boolalpha;
  try
  {
    demonstrateStack();
    demonstrateQueue();
    demonstrateDeque();
    demonstrateHeap();
    demonstrateErrors();
  }
  catch (const std::exception& error)
  {
    std::cerr << "Ошибка выполнения: " << error.what() << '\n';
    return 1;
  }
}
