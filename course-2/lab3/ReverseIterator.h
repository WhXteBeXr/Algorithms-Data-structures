#pragma once

#include <iterator>
#include <type_traits>

// Ptr равен T* или const T*. Итератор хранит позицию после своего элемента,
// поэтому rend() не требует создания недопустимого адреса перед массивом.
template <typename Ptr>
class ReverseIterator
{
public:
  using iterator_category = std::bidirectional_iterator_tag;
  using iterator_concept = std::bidirectional_iterator_tag;
  using value_type = typename std::iterator_traits<Ptr>::value_type;
  using difference_type = std::ptrdiff_t;
  using pointer = Ptr;
  using reference = typename std::iterator_traits<Ptr>::reference;

  explicit ReverseIterator(Ptr position = nullptr) : current(position) {}

  // Обычный итератор можно превратить в константный, обратное запрещено.
  template <typename OtherPtr>
    requires std::is_convertible_v<OtherPtr, Ptr>
  ReverseIterator(const ReverseIterator<OtherPtr>& other) : current(other.base()) {}

  Ptr base() const
  {
    return current;
  }

  reference operator*() const
  {
    // Разыменование допустимо только внутри обратного диапазона, не для rend().
    return *(current - 1);
  }

  pointer operator->() const
  {
    return current - 1;
  }

  ReverseIterator& operator++()
  {
    // Следующий элемент обратного обхода расположен левее в памяти.
    --current;
    return *this;
  }

  ReverseIterator operator++(int)
  {
    ReverseIterator previous = *this;
    ++(*this);
    return previous;
  }

  ReverseIterator& operator--()
  {
    ++current;
    return *this;
  }

  ReverseIterator operator--(int)
  {
    ReverseIterator previous = *this;
    --(*this);
    return previous;
  }

  template <typename OtherPtr>
  bool operator==(const ReverseIterator<OtherPtr>& other) const
  {
    return current == other.base();
  }

  template <typename OtherPtr>
  bool operator!=(const ReverseIterator<OtherPtr>& other) const
  {
    return !(*this == other);
  }

private:
  Ptr current;
};
