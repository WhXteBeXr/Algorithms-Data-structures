#pragma once

#include <limits>
#include <ostream>
#include <stdexcept>
#include <utility>

template <typename T>
std::size_t Vector<T>::maxSize()
{
  // Ограничиваем объём и разности адресов представимым знаковым размером.
  return static_cast<std::size_t>(std::numeric_limits<std::ptrdiff_t>::max()) / sizeof(T);
}

template <typename T>
std::size_t Vector<T>::checkedSize(std::ptrdiff_t requested)
{
  if (requested < 0)
  {
    throw std::invalid_argument("Размер и ёмкость не могут быть отрицательными");
  }
  const auto result = static_cast<std::size_t>(requested);
  if (result > maxSize())
  {
    throw std::length_error("Превышен допустимый размер Vector");
  }
  return result;
}

template <typename T>
std::size_t Vector<T>::nextCapacity() const
{
  if (buffer.capacity == maxSize())
  {
    throw std::length_error("Достигнута предельная ёмкость Vector");
  }
  if (buffer.capacity == 0)
  {
    return 1;
  }
  // Не вычисляем удвоение, если оно может выйти за разрешённый предел.
  return buffer.capacity > maxSize() / 2 ? maxSize() : buffer.capacity * 2;
}

template <typename T>
Vector<T>::Vector(std::ptrdiff_t count) : Vector(count, T{}) {}

template <typename T>
Vector<T>::Vector(std::ptrdiff_t count, const T& value) : buffer(checkedSize(count))
{
  while (buffer.size < buffer.capacity)
  {
    buffer.append(value);
  }
  // При ошибке копирования уже созданное поле buffer само освобождает ресурс.
}

template <typename T>
Vector<T>::Vector(const Vector& other) : buffer(other.size())
{
  // Глубокая копия: создаём собственные элементы в отдельном блоке памяти.
  for (const T& value : other)
  {
    buffer.append(value);
  }
}

template <typename T>
Vector<T>::Vector(Vector&& other) noexcept : copyCount(std::exchange(other.copyCount, 0))
{
  // Наш буфер изначально пуст. После обмена источник становится пустым.
  buffer.swap(other.buffer);
}

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector& other)
{
  if (this != &other)
  {
    Vector replacement(other);
    swap(replacement); // Старый буфер освободится при уничтожении replacement.
  }
  return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator=(Vector&& other) noexcept
{
  if (this != &other)
  {
    Vector replacement(std::move(other));
    swap(replacement);
  }
  return *this;
}

template <typename T>
T& Vector<T>::operator[](std::ptrdiff_t index)
{
  // Общая проверка находится в const-версии, чтобы условия не расходились.
  return const_cast<T&>(std::as_const(*this)[index]);
}

template <typename T>
const T& Vector<T>::operator[](std::ptrdiff_t index) const
{
  if (index < 0 || static_cast<std::size_t>(index) >= buffer.size)
  {
    throw std::out_of_range("Индекс вне границ Vector");
  }
  return buffer.data[index];
}

template <typename T>
std::size_t Vector<T>::size() const
{
  return buffer.size;
}

template <typename T>
std::size_t Vector<T>::capacity() const
{
  return buffer.capacity;
}

template <typename T>
bool Vector<T>::empty() const
{
  return buffer.size == 0;
}

template <typename T>
void Vector<T>::reserve(std::ptrdiff_t newCapacity)
{
  const std::size_t requested = checkedSize(newCapacity);
  if (requested <= buffer.capacity)
  {
    return;
  }

  vector_detail::VectorBuffer<T> replacement(requested);
  for (const T& value : *this)
  {
    replacement.append(value);
  }
  // Меняем основной буфер и статистику только после успешного копирования.
  copyCount += buffer.size;
  buffer.swap(replacement);
}

template <typename T>
void Vector<T>::push_back(const T& value)
{
  if (buffer.size == buffer.capacity)
  {
    vector_detail::VectorBuffer<T> replacement(nextCapacity());
    for (const T& existing : *this)
    {
      replacement.append(existing);
    }
    // value может ссылаться на наш элемент. Старый буфер пока ещё существует.
    replacement.append(value);
    copyCount += buffer.size;
    buffer.swap(replacement);
  }
  else
  {
    buffer.append(value);
  }
}

template <typename T>
void Vector<T>::pop_back()
{
  if (empty())
  {
    throw std::underflow_error("Vector пуст");
  }
  buffer.removeLast(); // Вызывается деструктор T, а не только уменьшается размер.
}

template <typename T>
void Vector<T>::resize(std::ptrdiff_t newSize, const T& value)
{
  const std::size_t requested = checkedSize(newSize);
  if (requested <= buffer.size)
  {
    while (buffer.size > requested)
    {
      buffer.removeLast();
    }
    return;
  }

  if (requested > buffer.capacity)
  {
    vector_detail::VectorBuffer<T> replacement(requested);
    for (const T& existing : *this)
    {
      replacement.append(existing);
    }
    // value остаётся допустимым даже при ссылке на элемент старого буфера.
    while (replacement.size < requested)
    {
      replacement.append(value);
    }
    copyCount += buffer.size;
    buffer.swap(replacement);
  }
  else
  {
    const std::size_t oldSize = buffer.size;
    try
    {
      while (buffer.size < requested)
      {
        buffer.append(value);
      }
    }
    catch (...)
    {
      // При ошибке роста без перевыделения удаляем только новые элементы.
      while (buffer.size > oldSize)
      {
        buffer.removeLast();
      }
      throw;
    }
  }
}

template <typename T>
void Vector<T>::clear() noexcept
{
  buffer.clear(); // Уничтожаем элементы, ёмкость сохраняется для повторного использования.
}

template <typename T>
void Vector<T>::swap(Vector& other) noexcept
{
  buffer.swap(other.buffer);
  std::swap(copyCount, other.copyCount);
}

template <typename T>
std::size_t Vector<T>::getCopyCount() const
{
  return copyCount;
}

template <typename T>
void Vector<T>::resetCopyCount()
{
  copyCount = 0;
}

template <typename T>
long double Vector<T>::sum() const requires std::is_arithmetic_v<T>
{
  // Расширенный тип накопления исключает переполнение арифметики самого int.
  long double result = 0;
  for (const T& value : *this)
  {
    result += static_cast<long double>(value);
  }
  return result;
}

template <typename T>
long double Vector<T>::average() const requires std::is_arithmetic_v<T>
{
  if (empty())
  {
    throw std::underflow_error("Среднее пустого Vector не определено");
  }
  return sum() / static_cast<long double>(buffer.size);
}

template <typename T>
void Vector<T>::print(std::ostream& out) const
{
  out << '[';
  for (std::size_t i = 0; i < buffer.size; ++i)
  {
    if (i != 0)
    {
      out << ", ";
    }
    out << buffer.data[i];
  }
  out << ']';
}

template <typename T>
typename Vector<T>::iterator Vector<T>::begin()
{
  return buffer.data;
}

template <typename T>
typename Vector<T>::iterator Vector<T>::end()
{
  // Не выполняем арифметику с nullptr у вектора, ещё не выделявшего память.
  return empty() ? buffer.data : buffer.data + buffer.size;
}

template <typename T>
typename Vector<T>::const_iterator Vector<T>::begin() const
{
  return buffer.data;
}

template <typename T>
typename Vector<T>::const_iterator Vector<T>::end() const
{
  return empty() ? buffer.data : buffer.data + buffer.size;
}

template <typename T>
typename Vector<T>::reverse_iterator Vector<T>::rbegin()
{
  return reverse_iterator(end());
}

template <typename T>
typename Vector<T>::reverse_iterator Vector<T>::rend()
{
  return reverse_iterator(begin());
}

template <typename T>
typename Vector<T>::const_reverse_iterator Vector<T>::rbegin() const
{
  return const_reverse_iterator(end());
}

template <typename T>
typename Vector<T>::const_reverse_iterator Vector<T>::rend() const
{
  return const_reverse_iterator(begin());
}
