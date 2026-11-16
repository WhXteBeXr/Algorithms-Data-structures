#pragma once

#include "BinaryTree.h"

class BST
{
  using Node = BinaryTree::Node;
  using Link = std::unique_ptr<Node>;
  BinaryTree tree;
  std::size_t count = 0;

  const Node* extreme(bool right) const;
  static const Node* select(const Node* node, std::size_t& remaining);

public:
  // Дубликаты не создают узлов. Результат сообщает об изменении дерева.
  bool insert(int key);
  bool search(int key) const;
  bool remove(int key);
  int kthLargest(std::ptrdiff_t k) const;
  int minimum() const { return extreme(false)->key; }
  int maximum() const { return extreme(true)->key; }
  std::size_t size() const { return count; }
  std::size_t height() const { return tree.height(); }
  std::vector<int> inOrder() const { return tree.inOrder(); }
  void print(std::ostream& out) const { tree.print(out); }
};
