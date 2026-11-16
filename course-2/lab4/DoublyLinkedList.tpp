#pragma once

#include <ostream>
#include <stdexcept>
#include <utility>

template <typename T>
DoublyLinkedList<T>::DoublyLinkedList(const DoublyLinkedList& other)
{
  // Частичная цепочка принадлежит локальному списку, который умеет её очистить.
  DoublyLinkedList replacement;
  for (const T& value : other)
  {
    replacement.push_back(value);
  }
  swap(replacement);
}

template <typename T>
DoublyLinkedList<T>::DoublyLinkedList(DoublyLinkedList&& other) noexcept
{
  swap(other);
}

template <typename T>
DoublyLinkedList<T>::~DoublyLinkedList()
{
  clear();
}

template <typename T>
DoublyLinkedList<T>& DoublyLinkedList<T>::operator=(const DoublyLinkedList& other)
{
  if (this != &other)
  {
    DoublyLinkedList replacement(other);
    swap(replacement); // Старая цепочка будет удалена деструктором replacement.
  }
  return *this;
}

template <typename T>
DoublyLinkedList<T>& DoublyLinkedList<T>::operator=(DoublyLinkedList&& other) noexcept
{
  if (this != &other)
  {
    DoublyLinkedList replacement(std::move(other));
    swap(replacement);
  }
  return *this;
}

template <typename T>
void DoublyLinkedList<T>::swap(DoublyLinkedList& other) noexcept
{
  // После обмена/перемещения прежние итераторы не используются: связь с
  // полем tail относится к старому владельцу. Сами T при обмене не копируются.
  std::swap(head, other.head);
  std::swap(tail, other.tail);
  std::swap(count, other.count);
  std::swap(lastAccessSteps, other.lastAccessSteps);
}

template <typename T>
void DoublyLinkedList<T>::push_front(const T& value)
{
  auto* added = new DNode<T>(value);
  added->next = head;
  if (head == nullptr)
  {
    tail = added;
  }
  else
  {
    head->prev = added; // У прежней головы появляется предшественник.
  }
  head = added;
  ++count;
}

template <typename T>
void DoublyLinkedList<T>::push_back(const T& value)
{
  auto* added = new DNode<T>(value);
  added->prev = tail;
  if (tail == nullptr)
  {
    head = added;
  }
  else
  {
    tail->next = added;
  }
  tail = added;
  ++count;
}

template <typename T>
void DoublyLinkedList<T>::validatePosition(ConstIterator position) const
{
  // Проверяем владельца за O(1), не добавляя скрытого обхода перед вставкой.
  if (position.tailLink != &tail)
  {
    throw std::invalid_argument("Итератор не принадлежит этому списку");
  }
}

template <typename T>
void DoublyLinkedList<T>::insert_after(ConstIterator position, const T& value)
{
  validatePosition(position);
  DNode<T>* left = position.current;
  if (left == nullptr)
  {
    throw std::out_of_range("После end нет узла для вставки");
  }
  auto* added = new DNode<T>(value);
  // Новый узел сначала получает обе связи с соседями.
  added->prev = left;
  added->next = left->next;
  if (left->next == nullptr)
  {
    tail = added;
  }
  else
  {
    left->next->prev = added; // Сосед справа должен ссылаться назад на новый узел.
  }
  left->next = added;
  ++count;
}

template <typename T>
void DoublyLinkedList<T>::insert_before(ConstIterator position, const T& value)
{
  validatePosition(position);
  DNode<T>* right = position.current;
  if (right == nullptr)
  {
    push_back(value);
    return;
  }
  if (right == head)
  {
    push_front(value);
    return;
  }
  auto* added = new DNode<T>(value);
  added->prev = right->prev;
  added->next = right;
  // Связь prev сразу даёт левого соседа, искать его от head не нужно.
  right->prev->next = added;
  right->prev = added;
  ++count;
}

template <typename T>
void DoublyLinkedList<T>::unlink(DNode<T>* node) noexcept
{
  // Перенаправляем связи с обеих сторон. Отсутствие соседа означает,
  // что удаляемый узел был головой или хвостом.
  if (node->prev == nullptr)
  {
    head = node->next;
  }
  else
  {
    node->prev->next = node->next;
  }
  if (node->next == nullptr)
  {
    tail = node->prev;
  }
  else
  {
    node->next->prev = node->prev;
  }
  delete node;
  --count;
}

template <typename T>
void DoublyLinkedList<T>::pop_front()
{
  if (empty())
  {
    throw std::underflow_error("Список пуст");
  }
  unlink(head);
}

template <typename T>
void DoublyLinkedList<T>::pop_back()
{
  if (empty())
  {
    throw std::underflow_error("Список пуст");
  }
  unlink(tail); // Предшественник уже доступен через prev, поиск не требуется.
}

template <typename T>
bool DoublyLinkedList<T>::erase(ConstIterator position)
{
  validatePosition(position);
  if (position.current == nullptr)
  {
    return false;
  }
  unlink(position.current);
  return true;
}

template <typename T>
bool DoublyLinkedList<T>::remove(const T& value)
{
  // find выполняет O(n) поиска, само исключение найденного узла занимает O(1).
  return erase(find(value));
}

template <typename T>
void DoublyLinkedList<T>::clear() noexcept
{
  while (head != nullptr)
  {
    DNode<T>* removed = head;
    head = head->next;
    delete removed;
  }
  // Во время очистки prev не используется, после неё обе границы пусты.
  tail = nullptr;
  count = 0;
  lastAccessSteps = 0;
}

template <typename T>
typename DoublyLinkedList<T>::Iterator DoublyLinkedList<T>::find(const T& value)
{
  for (auto it = begin(); it != end(); ++it)
  {
    if (*it == value)
    {
      return it;
    }
  }
  return end();
}

template <typename T>
typename DoublyLinkedList<T>::ConstIterator DoublyLinkedList<T>::find(const T& value) const
{
  for (auto it = begin(); it != end(); ++it)
  {
    if (*it == value)
    {
      return it;
    }
  }
  return end();
}

template <typename T>
DNode<T>* DoublyLinkedList<T>::nodeAt(std::ptrdiff_t index) const
{
  if (index < 0 || static_cast<std::size_t>(index) >= count)
  {
    throw std::out_of_range("Индекс вне границ списка");
  }
  lastAccessSteps = 0;
  const auto target = static_cast<std::size_t>(index);
  DNode<T>* current;
  if (target < count / 2)
  {
    current = head;
    for (std::size_t position = 0; position < target; ++position)
    {
      current = current->next;
      ++lastAccessSteps;
    }
  }
  else
  {
    current = tail;
    for (std::size_t position = count - 1; position > target; --position)
    {
      current = current->prev;
      ++lastAccessSteps;
    }
  }
  return current;
}

template <typename T>
T& DoublyLinkedList<T>::operator[](std::ptrdiff_t index)
{
  return nodeAt(index)->data;
}

template <typename T>
const T& DoublyLinkedList<T>::operator[](std::ptrdiff_t index) const
{
  return nodeAt(index)->data;
}

template <typename T>
std::size_t DoublyLinkedList<T>::getLastAccessSteps() const
{
  return lastAccessSteps;
}

template <typename T>
void DoublyLinkedList<T>::removeDuplicates()
{
  // Для каждого первого вхождения просматриваем оставшуюся часть списка.
  // Требуется только равенство T, сортировка или хеширование не нужны.
  for (DNode<T>* current = head; current != nullptr; current = current->next)
  {
    DNode<T>* runner = current->next;
    while (runner != nullptr)
    {
      DNode<T>* next = runner->next; // После unlink сам runner уже недействителен.
      if (runner->data == current->data)
      {
        unlink(runner);
      }
      runner = next;
    }
  }
}

template <typename T>
void DoublyLinkedList<T>::reverse() noexcept
{
  DNode<T>* current = head;
  while (current != nullptr)
  {
    std::swap(current->next, current->prev);
    current = current->prev; // После обмена здесь находится прежний next.
  }
  std::swap(head, tail);
}

template <typename T>
void DoublyLinkedList<T>::print(std::ostream& out) const
{
  out << '[';
  for (const DNode<T>* current = head; current != nullptr; current = current->next)
  {
    if (current != head)
    {
      out << ", ";
    }
    out << current->data;
  }
  out << ']';
}

template <typename T>
void DoublyLinkedList<T>::printReverse(std::ostream& out) const
{
  out << '[';
  for (const DNode<T>* current = tail; current != nullptr; current = current->prev)
  {
    if (current != tail)
    {
      out << ", ";
    }
    out << current->data;
  }
  out << ']';
}

template <typename T>
std::size_t DoublyLinkedList<T>::size() const
{
  return count;
}

template <typename T>
bool DoublyLinkedList<T>::empty() const
{
  return head == nullptr;
}

template <typename T>
typename DoublyLinkedList<T>::Iterator DoublyLinkedList<T>::begin()
{
  return Iterator(head, &tail);
}

template <typename T>
typename DoublyLinkedList<T>::Iterator DoublyLinkedList<T>::end()
{
  // Граница диапазона не указывает на элемент, но сохраняет связь с владельцем.
  return Iterator(nullptr, &tail);
}

template <typename T>
typename DoublyLinkedList<T>::ConstIterator DoublyLinkedList<T>::begin() const
{
  return ConstIterator(head, &tail);
}

template <typename T>
typename DoublyLinkedList<T>::ConstIterator DoublyLinkedList<T>::end() const
{
  return ConstIterator(nullptr, &tail);
}
