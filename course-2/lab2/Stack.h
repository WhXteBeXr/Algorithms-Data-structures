#pragma once

#include <cstddef>
#include <memory>
#include <optional>

// LIFO: последним добавлен, первым извлечён.
// T должен поддерживать копирование. Конструктор T по умолчанию не требуется.
template <typename T>
class Stack
{
public:
  explicit Stack(std::size_t initialCapacity = 10);
  ~Stack() = default;

  // Стек единолично владеет буфером. Копирование и перемещение в работе не реализуются.
  Stack(const Stack&) = delete;
  Stack& operator=(const Stack&) = delete;
  Stack(Stack&&) = delete;
  Stack& operator=(Stack&&) = delete;

  void push(const T& value);
  T pop();
  const T& top() const;
  bool isEmpty() const;
  std::size_t size() const;

private:
  // optional отличает свободную ячейку от живого T. reset() уничтожает
  // извлечённый элемент сразу, не дожидаясь уничтожения всего стека.
  std::unique_ptr<std::optional<T>[]> data;
  std::size_t elementCount = 0;
  std::size_t bufferCapacity;
};

// Определения шаблона нужны компилятору в месте использования Stack<T>.
#include "Stack.tpp"
