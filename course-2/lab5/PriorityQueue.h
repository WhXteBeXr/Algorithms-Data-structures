#pragma once

#include <cstddef>
#include <memory>

class PriorityQueue
{
public:
  PriorityQueue() = default;
  PriorityQueue(const PriorityQueue&) = delete;
  PriorityQueue& operator=(const PriorityQueue&) = delete;

  void push(int value);
  int pop();
  int top() const;
  bool isEmpty() const { return count == 0; }
  std::size_t size() const { return count; }

private:
  std::unique_ptr<int[]> heap;
  std::size_t count = 0;
  std::size_t capacity = 0;
  void heapifyUp(std::size_t index);
  void heapifyDown(std::size_t index);
};
