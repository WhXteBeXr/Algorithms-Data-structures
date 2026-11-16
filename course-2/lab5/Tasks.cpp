#include "Tasks.h"
#include "PriorityQueue.h"
#include "Stack.h"

std::string toBinary(std::int64_t number)
{
  if (number == 0)
  {
    return "0";
  }
  // Такая запись модуля не вычисляет -INT64_MIN, которое не помещается в int64_t.
  std::uint64_t magnitude = number < 0 ? static_cast<std::uint64_t>(-(number + 1)) + 1
                                      : static_cast<std::uint64_t>(number);
  Stack digits;
  while (magnitude != 0)
  {
    digits.push(static_cast<int>(magnitude % 2));
    magnitude /= 2;
  }
  std::string result = number < 0 ? "-" : "";
  while (!digits.isEmpty())
  {
    result += static_cast<char>('0' + digits.pop()); // Старшие биты выходят первыми.
  }
  return result;
}

void heapSort(std::span<int> values)
{
  PriorityQueue queue;
  for (int value : values)
  {
    queue.push(value);
  }
  // Max-heap выдаёт значения по убыванию. Запись с конца даёт возрастание.
  for (std::size_t i = values.size(); i > 0; --i)
  {
    values[i - 1] = queue.pop();
  }
}
