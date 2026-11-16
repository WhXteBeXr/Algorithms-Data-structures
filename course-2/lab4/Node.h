#pragma once

// Узел хранит значение отдельно от остальных узлов. Адреса связей могут быть
// nullptr, но сам узел всегда содержит уже созданный объект T.
template <typename T>
struct Node
{
  T data;
  Node* next = nullptr;

  explicit Node(const T& value) : data(value) {}
};

template <typename T>
struct DNode
{
  T data;
  DNode* prev = nullptr;
  DNode* next = nullptr;

  explicit DNode(const T& value) : data(value) {}
};
