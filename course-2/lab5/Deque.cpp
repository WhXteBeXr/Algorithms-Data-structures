#include "Deque.h"

#include <algorithm>
#include <stdexcept>

void Deque::refill(Stack& target, Stack& source)
{
  // target пуст. Оставляем половину элементов на стороне source, чтобы
  // чередование удалений с двух концов не переносило весь дек каждый раз.
  const std::size_t saved = source.size() / 2;
  Stack temporary;
  temporary.reserve(saved);
  target.reserve(source.size() - saved);
  // Всё необходимое место выделено заранее. Дальше push уже не выделяет память,
  // поэтому ошибка выделения не может оставить частично переставленные данные.
  for (std::size_t i = 0; i < saved; ++i)
  {
    temporary.push(source.pop());
  }
  while (!source.isEmpty())
  {
    target.push(source.pop());
  }
  while (!temporary.isEmpty())
  {
    source.push(temporary.pop());
  }
}

void Deque::push_front(int value)
{
  left.push(value);
}
void Deque::push_back(int value)
{
  right.push(value);
}

int Deque::front() const
{
  if (isEmpty())
  {
    throw std::underflow_error("Дек пуст");
  }
  // Если левый стек пуст, начало находится на дне правого. Для просмотра
  // перестановка не нужна, достаточно прямого доступа к первой записи.
  return left.isEmpty() ? right.data[0].value : left.top();
}

int Deque::back() const
{
  if (isEmpty())
  {
    throw std::underflow_error("Дек пуст");
  }
  return right.isEmpty() ? left.data[0].value : right.top();
}

int Deque::pop_front()
{
  front(); // Отклоняем пустой дек до перестановок.
  if (left.isEmpty())
  {
    refill(left, right);
  }
  return left.pop();
}

int Deque::pop_back()
{
  back();
  if (right.isEmpty())
  {
    refill(right, left);
  }
  return right.pop();
}

int Deque::getMax() const
{
  if (isEmpty())
  {
    throw std::underflow_error("Дек пуст");
  }
  if (left.isEmpty())
  {
    return right.getMax();
  }
  if (right.isEmpty())
  {
    return left.getMax();
  }
  return std::max(left.getMax(), right.getMax());
}
