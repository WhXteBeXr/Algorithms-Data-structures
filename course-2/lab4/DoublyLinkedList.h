#pragma once

#include "ListIterator.h"
#include "Node.h"

#include <cstddef>
#include <iosfwd>

template <typename T>
class DoublyLinkedList
{
public:
  using Iterator = ListIterator<DNode<T>, false, true>;
  using ConstIterator = ListIterator<DNode<T>, true, true>;

  DoublyLinkedList() = default;
  DoublyLinkedList(const DoublyLinkedList& other);
  DoublyLinkedList(DoublyLinkedList&& other) noexcept;
  ~DoublyLinkedList();
  DoublyLinkedList& operator=(const DoublyLinkedList& other);
  DoublyLinkedList& operator=(DoublyLinkedList&& other) noexcept;
  void swap(DoublyLinkedList& other) noexcept;

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

  Iterator find(const T& value);
  ConstIterator find(const T& value) const;
  T& operator[](std::ptrdiff_t index);
  const T& operator[](std::ptrdiff_t index) const;
  // Число переходов next/prev при последнем успешном operator[], без проверки индекса.
  std::size_t getLastAccessSteps() const;

  void removeDuplicates();
  void reverse() noexcept;
  void print(std::ostream& out) const;
  void printReverse(std::ostream& out) const;
  std::size_t size() const;
  bool empty() const;
  // Вставка сохраняет итераторы на существующие узлы. После удаления узла,
  // clear, swap и перемещения недействительные итераторы не используются.
  Iterator begin();
  Iterator end();
  ConstIterator begin() const;
  ConstIterator end() const;

private:
  DNode<T>* head = nullptr;
  DNode<T>* tail = nullptr;
  std::size_t count = 0;
  mutable std::size_t lastAccessSteps = 0;

  DNode<T>* nodeAt(std::ptrdiff_t index) const;
  void validatePosition(ConstIterator position) const;
  void unlink(DNode<T>* node) noexcept;
};

#include "DoublyLinkedList.tpp"
