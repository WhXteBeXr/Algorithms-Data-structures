#pragma once

#include "Stack.h"

class Deque
{
public:
  void push_front(int value);
  void push_back(int value);
  int pop_front();
  int pop_back();
  int front() const;
  int back() const;
  int getMax() const;
  bool isEmpty() const { return left.isEmpty() && right.isEmpty(); }
  std::size_t size() const { return left.size() + right.size(); }

private:
  // Вершина left - начало дека, вершина right - конец.
  Stack left;
  Stack right;
  static void refill(Stack& target, Stack& source);
};
