#pragma once

#include <algorithm>
#include <cstddef>
#include <limits>
#include <memory>
#include <stdexcept>
#include <utility>

namespace lab5_detail
{
  // Общий помощник только для массивов int и простых записей стека.
  // Удвоение сокращает число выделений, unique_ptr освобождает старый массив.
  template <typename T>
  void growArray(std::unique_ptr<T[]>& data, std::size_t& capacity,
                 std::size_t count, std::size_t required)
  {
    if (required <= capacity)
    {
      return;
    }
    const std::size_t limit = std::numeric_limits<std::size_t>::max() / sizeof(T);
    if (required > limit)
    {
      throw std::length_error("Превышен размер массива");
    }
    const std::size_t next = capacity > limit / 2 ? required : std::max(required, capacity == 0 ? 4 : capacity * 2);
    auto replacement = std::make_unique<T[]>(next);
    if (count != 0)
    {
      std::copy_n(data.get(), count, replacement.get());
    }
    // До этой строки исходный массив сохранён даже при ошибке выделения памяти.
    data = std::move(replacement);
    capacity = next;
  }
}
