#pragma once

#include <ostream>
#include <stdexcept>
#include <utility>

template <typename T>
SinglyLinkedList<T>::SinglyLinkedList(const SinglyLinkedList& other)
{
  // Локальный владелец удалит частичную копию, если копирование T прервётся.
  SinglyLinkedList replacement;
  for (const T& value : other)
  {
    replacement.push_back(value);
  }
  swap(replacement);
}

template <typename T>
SinglyLinkedList<T>::SinglyLinkedList(SinglyLinkedList&& other) noexcept
{
  swap(other); // Наши начальные поля пусты, поэтому источник станет пустым.
}

template <typename T>
SinglyLinkedList<T>::~SinglyLinkedList()
{
  clear();
}

template <typename T>
SinglyLinkedList<T>& SinglyLinkedList<T>::operator=(const SinglyLinkedList& other)
{
  if (this != &other)
  {
    SinglyLinkedList replacement(other);
    swap(replacement); // Старые узлы теперь принадлежат локальному replacement.
  }
  return *this;
}

template <typename T>
SinglyLinkedList<T>& SinglyLinkedList<T>::operator=(SinglyLinkedList&& other) noexcept
{
  if (this != &other)
  {
    SinglyLinkedList replacement(std::move(other));
    swap(replacement);
  }
  return *this;
}

template <typename T>
void SinglyLinkedList<T>::swap(SinglyLinkedList& other) noexcept
{
  // Меняем владельцев цепочек без копирования T. Старые итераторы после swap
  // не используются: их сведения о владельце относятся к прежнему объекту.
  std::swap(head, other.head);
  std::swap(tail, other.tail);
  std::swap(count, other.count);
  std::swap(lastAccessSteps, other.lastAccessSteps);
}

template <typename T>
void SinglyLinkedList<T>::push_front(const T& value)
{
  // Создаём значение до изменения связей. При ошибке new список не изменён.
  auto* added = new Node<T>(value);
  added->next = head;
  head = added;
  if (tail == nullptr)
  {
    tail = added; // Первый узел одновременно является последним.
  }
  ++count;
}

template <typename T>
void SinglyLinkedList<T>::push_back(const T& value)
{
  auto* added = new Node<T>(value);
  if (tail == nullptr)
  {
    head = added;
  }
  else
  {
    tail->next = added; // Прежний хвост теперь связан с новым узлом.
  }
  tail = added;
  ++count;
}

template <typename T>
void SinglyLinkedList<T>::validatePosition(ConstIterator position) const
{
  if (position.tailLink != &tail)
  {
    throw std::invalid_argument("Итератор не принадлежит этому списку");
  }
}

template <typename T>
void SinglyLinkedList<T>::insert_after(ConstIterator position, const T& value)
{
  validatePosition(position);
  if (position.current == nullptr)
  {
    throw std::out_of_range("После end нет узла для вставки");
  }
  auto* added = new Node<T>(value);
  // Сначала сохраняем продолжение цепочки в новом узле, затем подключаем узел.
  added->next = position.current->next;
  position.current->next = added;
  if (tail == position.current)
  {
    tail = added;
  }
  ++count;
}

template <typename T>
void SinglyLinkedList<T>::insert_before(ConstIterator position, const T& value)
{
  validatePosition(position);
  if (position.current == nullptr)
  {
    push_back(value); // Вставка перед end означает добавление в конец.
    return;
  }
  if (position.current == head)
  {
    push_front(value);
    return;
  }
  // Обратной ссылки нет: предшественника приходится искать от головы.
  Node<T>* previous = head;
  while (previous->next != position.current)
  {
    previous = previous->next;
  }
  auto* added = new Node<T>(value);
  added->next = position.current;
  previous->next = added;
  ++count;
}

template <typename T>
void SinglyLinkedList<T>::unlink(Node<T>* previous, Node<T>* node) noexcept
{
  // Закрытый помощник получает живой узел и его известного предшественника.
  if (previous == nullptr)
  {
    head = node->next;
  }
  else
  {
    previous->next = node->next; // Исключаем узел из цепочки до освобождения памяти.
  }
  if (node == tail)
  {
    tail = previous; // При удалении единственного узла previous равен nullptr.
  }
  delete node;
  --count;
}

template <typename T>
void SinglyLinkedList<T>::pop_front()
{
  if (empty())
  {
    throw std::underflow_error("Список пуст");
  }
  unlink(nullptr, head);
}

template <typename T>
void SinglyLinkedList<T>::pop_back()
{
  if (empty())
  {
    throw std::underflow_error("Список пуст");
  }
  Node<T>* previous = nullptr;
  if (head != tail)
  {
    previous = head;
    while (previous->next != tail)
    {
      previous = previous->next;
    }
  }
  unlink(previous, tail);
}

template <typename T>
bool SinglyLinkedList<T>::erase(ConstIterator position)
{
  validatePosition(position);
  if (position.current == nullptr)
  {
    return false; // end допустим как обозначение отсутствующего результата поиска.
  }
  Node<T>* previous = nullptr;
  if (position.current != head)
  {
    previous = head;
    while (previous->next != position.current)
    {
      previous = previous->next;
    }
  }
  unlink(previous, position.current);
  return true;
}

template <typename T>
bool SinglyLinkedList<T>::remove(const T& value)
{
  // Ищем узел и его предшественника одновременно, чтобы не проходить список дважды.
  Node<T>* previous = nullptr;
  Node<T>* current = head;
  while (current != nullptr && !(current->data == value))
  {
    previous = current;
    current = current->next;
  }
  if (current == nullptr)
  {
    return false;
  }
  unlink(previous, current);
  return true;
}

template <typename T>
void SinglyLinkedList<T>::clear() noexcept
{
  while (head != nullptr)
  {
    Node<T>* removed = head;
    head = head->next; // Адрес следующего узла нужен до удаления текущего.
    delete removed;
  }
  tail = nullptr;
  count = 0;
  lastAccessSteps = 0;
}

template <typename T>
typename SinglyLinkedList<T>::Iterator SinglyLinkedList<T>::find(const T& value)
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
typename SinglyLinkedList<T>::ConstIterator SinglyLinkedList<T>::find(const T& value) const
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
const T& SinglyLinkedList<T>::findMax() const
{
  if (empty())
  {
    throw std::underflow_error("Максимум пустого списка не определён");
  }
  const Node<T>* maximum = head;
  for (const Node<T>* current = head->next; current != nullptr; current = current->next)
  {
    if (maximum->data < current->data)
    {
      maximum = current;
    }
  }
  return maximum->data;
}

template <typename T>
Node<T>* SinglyLinkedList<T>::nodeAt(std::ptrdiff_t index) const
{
  if (index < 0 || static_cast<std::size_t>(index) >= count)
  {
    throw std::out_of_range("Индекс вне границ списка");
  }
  lastAccessSteps = 0;
  Node<T>* current = head;
  for (std::ptrdiff_t position = 0; position < index; ++position)
  {
    current = current->next;
    ++lastAccessSteps; // Считаем именно переходы по связям, а не проверки условий.
  }
  return current;
}

template <typename T>
T& SinglyLinkedList<T>::operator[](std::ptrdiff_t index)
{
  return nodeAt(index)->data;
}

template <typename T>
const T& SinglyLinkedList<T>::operator[](std::ptrdiff_t index) const
{
  return nodeAt(index)->data;
}

template <typename T>
std::size_t SinglyLinkedList<T>::getLastAccessSteps() const
{
  return lastAccessSteps;
}

template <typename T>
void SinglyLinkedList<T>::reverse() noexcept
{
  Node<T>* previous = nullptr;
  Node<T>* current = head;
  tail = head; // Прежняя голова станет хвостом после переворота всех связей.
  while (current != nullptr)
  {
    Node<T>* next = current->next;
    current->next = previous;
    previous = current;
    current = next; // Продолжаем по сохранённой, ещё не перевёрнутой цепочке.
  }
  head = previous;
}

template <typename T>
void SinglyLinkedList<T>::print(std::ostream& out) const
{
  out << '[';
  for (const Node<T>* current = head; current != nullptr; current = current->next)
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
std::size_t SinglyLinkedList<T>::size() const
{
  return count;
}

template <typename T>
bool SinglyLinkedList<T>::empty() const
{
  return head == nullptr;
}

template <typename T>
typename SinglyLinkedList<T>::Iterator SinglyLinkedList<T>::begin()
{
  return Iterator(head, &tail);
}

template <typename T>
typename SinglyLinkedList<T>::Iterator SinglyLinkedList<T>::end()
{
  // Граница диапазона не указывает на элемент, но сохраняет связь с владельцем.
  return Iterator(nullptr, &tail);
}

template <typename T>
typename SinglyLinkedList<T>::ConstIterator SinglyLinkedList<T>::begin() const
{
  return ConstIterator(head, &tail);
}

template <typename T>
typename SinglyLinkedList<T>::ConstIterator SinglyLinkedList<T>::end() const
{
  return ConstIterator(nullptr, &tail);
}
