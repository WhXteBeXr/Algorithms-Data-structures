#pragma once

#include <cstddef>
#include <memory>

class Deque;

// Стек целых чисел. Сохранённые максимумы также используются очередью и деком.
class Stack
{
public:
  Stack() = default;
  Stack(const Stack&) = delete;
  Stack& operator=(const Stack&) = delete;

  void push(int value);
  int pop();
  int top() const;
  int getMax() const;
  bool isEmpty() const { return count == 0; }
  std::size_t size() const { return count; }

private:
  struct Entry
  {
    int value;
    int maximum; // Максимум от дна до этой записи включительно.
  };
  std::unique_ptr<Entry[]> data;
  std::size_t count = 0;
  std::size_t capacity = 0;

  void reserve(std::size_t required);
  friend class Deque;
};
