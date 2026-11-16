#pragma once

#include "ListIterator.h"
#include "Node.h"

#include <cstddef>
#include <iosfwd>

// Список владеет узлами. T должен поддерживать используемые операции:
// копирование, равенство для поиска и сравнение для findMax.
template <typename T>
class SinglyLinkedList
{
public:
  using Iterator = ListIterator<Node<T>, false, false>;
  using ConstIterator = ListIterator<Node<T>, true, false>;

  SinglyLinkedList() = default;
  SinglyLinkedList(const SinglyLinkedList& other);
  SinglyLinkedList(SinglyLinkedList&& other) noexcept;
  ~SinglyLinkedList();
  SinglyLinkedList& operator=(const SinglyLinkedList& other);
  SinglyLinkedList& operator=(SinglyLinkedList&& other) noexcept;
  void swap(SinglyLinkedList& other) noexcept;

  void push_front(const T& value);
  void push_back(const T& value);
  // position должен быть живым итератором этого списка. После end вставка запрещена.
  void insert_after(ConstIterator position, const T& value);
  // Вставка перед end добавляет в конец, erase(end) возвращает false.
  void insert_before(ConstIterator position, const T& value);
  void pop_front();
  void pop_back();
  bool erase(ConstIterator position);
  bool remove(const T& value);
  void clear() noexcept;

  // Ненайденное значение обозначается end(), а не раскрытым указателем на узел.
  Iterator find(const T& value);
  ConstIterator find(const T& value) const;
  const T& findMax() const;
  T& operator[](std::ptrdiff_t index);
  const T& operator[](std::ptrdiff_t index) const;
  // Число переходов next/prev при последнем успешном operator[], без проверки индекса.
  std::size_t getLastAccessSteps() const;

  void reverse() noexcept;
  void print(std::ostream& out) const;
  std::size_t size() const;
  bool empty() const;
  // Вставка сохраняет итераторы на существующие узлы. После удаления узла,
  // clear, swap и перемещения недействительные итераторы не используются.
  Iterator begin();
  Iterator end();
  ConstIterator begin() const;
  ConstIterator end() const;

private:
  Node<T>* head = nullptr;
  Node<T>* tail = nullptr;
  std::size_t count = 0;
  // Измерение не меняет содержимое списка, поэтому доступно и при const-чтении.
  mutable std::size_t lastAccessSteps = 0;

  Node<T>* nodeAt(std::ptrdiff_t index) const;
  void validatePosition(ConstIterator position) const;
  void unlink(Node<T>* previous, Node<T>* node) noexcept;
};

#include "SinglyLinkedList.tpp"
