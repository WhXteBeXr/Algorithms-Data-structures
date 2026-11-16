#pragma once

#include <cstddef>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <type_traits>

template <typename T>
class SinglyLinkedList;
template <typename T>
class DoublyLinkedList;

// Один шаблон описывает const/обычный итератор и одно/двунаправленный обход.
// Узел скрыт от пользователя, чтобы нельзя было изменить связи через find().
template <typename NodeType, bool IsConst, bool IsDoubly>
class ListIterator
{
public:
  using iterator_category = std::conditional_t<IsDoubly, std::bidirectional_iterator_tag, std::forward_iterator_tag>;
  using iterator_concept = iterator_category;
  using value_type = std::remove_cvref_t<decltype(NodeType::data)>;
  using difference_type = std::ptrdiff_t;
  using pointer = std::conditional_t<IsConst, const value_type*, value_type*>;
  using reference = std::conditional_t<IsConst, const value_type&, value_type&>;

  ListIterator() = default;

  // Разрешён переход от изменяемого доступа к чтению, но не наоборот.
  template <bool OtherConst>
    requires (IsConst && !OtherConst)
  ListIterator(const ListIterator<NodeType, OtherConst, IsDoubly>& other) :
      current(other.current), tailLink(other.tailLink) {}

  reference operator*() const
  {
    if (current == nullptr)
    {
      throw std::out_of_range("Нельзя разыменовать end или пустой итератор");
    }
    return current->data;
  }

  pointer operator->() const
  {
    return std::addressof(operator*());
  }

  ListIterator& operator++()
  {
    if (current == nullptr)
    {
      throw std::out_of_range("Нельзя выполнить ++end");
    }
    current = current->next;
    return *this;
  }

  ListIterator operator++(int)
  {
    ListIterator previous = *this;
    ++(*this);
    return previous;
  }

  ListIterator& operator--() requires IsDoubly
  {
    if (current == nullptr)
    {
      // Храним адрес поля tail, а не копию старого хвоста. Поэтому сохранённый
      // end() учитывает новый хвост после push_back и pop_back.
      if (tailLink == nullptr || *tailLink == nullptr)
      {
        throw std::out_of_range("Нельзя выполнить --end пустого списка");
      }
      current = *tailLink;
    }
    else
    {
      if (current->prev == nullptr)
      {
        throw std::out_of_range("Нельзя выполнить --begin");
      }
      current = current->prev;
    }
    return *this;
  }

  ListIterator operator--(int) requires IsDoubly
  {
    ListIterator previous = *this;
    --(*this);
    return previous;
  }

  template <bool OtherConst>
  bool operator==(const ListIterator<NodeType, OtherConst, IsDoubly>& other) const
  {
    // Конечные итераторы разных списков различаются владельцем.
    return current == other.current && tailLink == other.tailLink;
  }

  template <bool OtherConst>
  bool operator!=(const ListIterator<NodeType, OtherConst, IsDoubly>& other) const
  {
    return !(*this == other);
  }

private:
  NodeType* current = nullptr;
  NodeType* const* tailLink = nullptr;

  ListIterator(NodeType* node, NodeType* const* ownerTail) : current(node), tailLink(ownerTail) {}

  template <typename>
  friend class SinglyLinkedList;
  template <typename>
  friend class DoublyLinkedList;
  template <typename, bool, bool>
  friend class ListIterator;
};
