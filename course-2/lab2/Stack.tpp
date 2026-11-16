#pragma once

#include <limits>
#include <stdexcept>
#include <utility>

template <typename T>
Stack<T>::Stack(std::size_t initialCapacity) : bufferCapacity(initialCapacity == 0 ? 1 : initialCapacity)
{
  // Нулевая начальная ёмкость превращается в единицу, чтобы удвоение давало рост.
  data = std::make_unique<std::optional<T>[]>(bufferCapacity);
}

template <typename T>
void Stack<T>::push(const T& value)
{
  if (elementCount == bufferCapacity)
  {
    // Проверяем удвоение и размер массива до вычисления новой ёмкости.
    const std::size_t maxCapacity = std::numeric_limits<std::size_t>::max() / sizeof(std::optional<T>);
    if (bufferCapacity > maxCapacity / 2)
    {
      throw std::length_error("Достигнута предельная ёмкость стека");
    }

    const std::size_t newCapacity = bufferCapacity * 2;
    auto newData = std::make_unique<std::optional<T>[]>(newCapacity);
    for (std::size_t i = 0; i < elementCount; ++i)
    {
      newData[i].emplace(*data[i]);
    }

    // value может ссылаться на top() этого же стека. Старый буфер сохраняем
    // до копирования нового элемента, иначе такая ссылка стала бы висячей.
    newData[elementCount].emplace(value);

    // Если выделение или копирование выбросили исключение, старый стек не изменён.
    // unique_ptr автоматически освобождает временный буфер при ошибке.
    data = std::move(newData);
    bufferCapacity = newCapacity;
  }
  else
  {
    data[elementCount].emplace(value);
  }
  ++elementCount; // Увеличиваем размер только после успешного создания элемента.
}

template <typename T>
T Stack<T>::pop()
{
  if (isEmpty())
  {
    throw std::out_of_range("Стек пуст");
  }
  // Сначала получаем возвращаемое значение, затем уничтожаем вершину в буфере.
  T value = *data[elementCount - 1];
  data[elementCount - 1].reset();
  --elementCount;
  return value;
}

template <typename T>
const T& Stack<T>::top() const
{
  if (isEmpty())
  {
    throw std::out_of_range("Стек пуст");
  }
  // Ссылка позволяет посмотреть вершину без копирования и без удаления.
  // Она недействительна после удаления элемента, расширения или уничтожения стека.
  return *data[elementCount - 1];
}

template <typename T>
bool Stack<T>::isEmpty() const
{
  return elementCount == 0;
}

template <typename T>
std::size_t Stack<T>::size() const
{
  return elementCount;
}
