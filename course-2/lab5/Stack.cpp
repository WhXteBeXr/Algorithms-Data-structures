#include "Stack.h"
#include "ArrayGrowth.h"

#include <algorithm>
#include <stdexcept>

void Stack::reserve(std::size_t required)
{
  lab5_detail::growArray(data, capacity, count, required);
}

void Stack::push(int value)
{
  reserve(count + 1);
  // Каждая вершина помнит максимум всего оставшегося под ней префикса.
  const int maximum = isEmpty() ? value : std::max(value, data[count - 1].maximum);
  data[count++] = {value, maximum};
}

int Stack::top() const
{
  if (isEmpty())
  {
    throw std::underflow_error("Стек пуст");
  }
  return data[count - 1].value;
}

int Stack::pop()
{
  const int value = top(); // Проверка пустоты выполняется до изменения размера.
  --count;
  return value;
}

int Stack::getMax() const
{
  if (isEmpty())
  {
    throw std::underflow_error("Стек пуст");
  }
  return data[count - 1].maximum;
}
