#pragma once

#include <cstddef>
#include <memory>
#include <utility>

namespace vector_detail
{
  // Внутренний владелец памяти. Только первые size ячеек содержат живые T.
  // Оставшаяся capacity - свободная память, конструкторы T для неё не вызываются.
  template <typename T>
  struct VectorBuffer
  {
    T* data = nullptr;
    std::size_t size = 0;
    std::size_t capacity = 0;

    explicit VectorBuffer(std::size_t capacity = 0) : capacity(capacity)
    {
      if (capacity != 0)
      {
        data = std::allocator<T>{}.allocate(capacity);
      }
    }

    VectorBuffer(const VectorBuffer&) = delete;
    VectorBuffer& operator=(const VectorBuffer&) = delete;

    ~VectorBuffer()
    {
      clear();
      if (data != nullptr)
      {
        // Освобождаем блок тем же способом, которым он был выделен.
        std::allocator<T>{}.deallocate(data, capacity);
      }
    }

    void append(const T& value)
    {
      // Вызывающий Vector заранее обеспечивает свободное место.
      // При ошибке конструктора размер остаётся прежним.
      std::construct_at(data + size, value);
      ++size;
    }

    void removeLast() noexcept
    {
      // Вызывающий код гарантирует size > 0. Память ячейки остаётся в буфере.
      --size;
      std::destroy_at(data + size);
    }

    void clear() noexcept
    {
      while (size != 0)
      {
        removeLast();
      }
    }

    void swap(VectorBuffer& other) noexcept
    {
      // Переставляем описание блоков, сами элементы не копируем.
      std::swap(data, other.data);
      std::swap(size, other.size);
      std::swap(capacity, other.capacity);
    }
  };
}
