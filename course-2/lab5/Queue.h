#pragma once

#include "Deque.h"

// Узкая обёртка повторно использует собственный дек: добавление только справа,
// извлечение только слева. Короткие делегирующие методы оставлены в заголовке.
class Queue
{
public:
  void enqueue(int value) { data.push_back(value); }
  int dequeue() { return data.pop_front(); }
  int front() const { return data.front(); }
  int getMax() const { return data.getMax(); }
  bool isEmpty() const { return data.isEmpty(); }
  std::size_t size() const { return data.size(); }

private:
  Deque data;
};
