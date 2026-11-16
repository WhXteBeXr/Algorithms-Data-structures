#pragma once

#include "ReverseIterator.h"
#include "VectorBuffer.h"

#include <cstddef>
#include <iosfwd>
#include <type_traits>

// Учебный непрерывный динамический массив. T должен поддерживать копирование,
// а его деструктор не должен выбрасывать исключения.
template <typename T>
class Vector
{
public:
  using iterator = T*;
  using const_iterator = const T*;
  using reverse_iterator = ReverseIterator<T*>;
  using const_reverse_iterator = ReverseIterator<const T*>;

  Vector() = default;
  explicit Vector(std::ptrdiff_t count);
  Vector(std::ptrdiff_t count, const T& value);
  Vector(const Vector& other);
  Vector(Vector&& other) noexcept;
  ~Vector() = default;

  Vector& operator=(const Vector& other);
  Vector& operator=(Vector&& other) noexcept;

  // В этой реализации operator[] проверяет границы, включая отрицательный индекс.
  T& operator[](std::ptrdiff_t index);
  const T& operator[](std::ptrdiff_t index) const;
  std::size_t size() const;
  std::size_t capacity() const;
  bool empty() const;

  void push_back(const T& value);
  void pop_back();
  void reserve(std::ptrdiff_t newCapacity);
  // Версия без value требует, чтобы T можно было создать как T{}.
  void resize(std::ptrdiff_t newSize, const T& value = T{});
  void clear() noexcept;
  void swap(Vector& other) noexcept;

  // Считаются только копирования существующих элементов при смене буфера.
  // История переносится вместе с буфером при move/swap, копия начинает с нуля.
  std::size_t getCopyCount() const;
  void resetCopyCount();

  long double sum() const requires std::is_arithmetic_v<T>;
  long double average() const requires std::is_arithmetic_v<T>;
  void print(std::ostream& out) const;

  iterator begin();
  iterator end();
  const_iterator begin() const;
  const_iterator end() const;
  reverse_iterator rbegin();
  reverse_iterator rend();
  const_reverse_iterator rbegin() const;
  const_reverse_iterator rend() const;

private:
  vector_detail::VectorBuffer<T> buffer;
  std::size_t copyCount = 0;

  static std::size_t checkedSize(std::ptrdiff_t requested);
  static std::size_t maxSize();
  std::size_t nextCapacity() const;
};

// Методы шаблона должны быть видны при создании Vector<T> в другом файле.
#include "Vector.tpp"
