#pragma once

#include <cstddef>
#include <iosfwd>
#include <memory>
#include <span>
#include <vector>

class BST;

class BinaryTree
{
  // Каждый узел единолично владеет детьми, корнем владеет само дерево.
  struct Node
  {
    int key;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;

    explicit Node(int value) : key(value) {}
  };

  enum class Order { Pre, In, Post };
  std::unique_ptr<Node> root;

  static std::unique_ptr<Node> build(std::span<const int> values, std::size_t index);
  static void visit(const Node* node, Order order, std::vector<int>& result);
  static std::size_t height(const Node* node);
  static void print(const Node* node, std::ostream& out, std::size_t depth, char side);
  std::vector<int> traverse(Order order) const;

  // BST переиспользует узлы и обходы, но сам поддерживает порядок ключей.
  friend class BST;

public:
  BinaryTree() = default;
  explicit BinaryTree(std::span<const int> values);
  BinaryTree(const BinaryTree&) = delete;
  BinaryTree& operator=(const BinaryTree&) = delete;

  std::vector<int> preOrder() const { return traverse(Order::Pre); }
  std::vector<int> inOrder() const { return traverse(Order::In); }
  std::vector<int> postOrder() const { return traverse(Order::Post); }
  // Высота измеряется уровнями: пустое дерево 0, один узел 1.
  std::size_t height() const { return height(root.get()); }
  void print(std::ostream& out) const;
};
