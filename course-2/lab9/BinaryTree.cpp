#include "BinaryTree.h"

#include <algorithm>
#include <ostream>
#include <string>

BinaryTree::BinaryTree(std::span<const int> values) : root(build(values, 0)) {}

std::unique_ptr<BinaryTree::Node> BinaryTree::build(std::span<const int> values, std::size_t index)
{
  if (index >= values.size())
  {
    return nullptr;
  }

  // Массив задаёт форму по уровням, а не порядок вставки в BST.
  auto node = std::make_unique<Node>(values[index]);
  // Проверяем наличие детей до вычисления индексов, исключая переполнение.
  if (index < values.size() / 2)
  {
    node->left = build(values, 2 * index + 1);
  }
  if (index < (values.size() - 1) / 2)
  {
    node->right = build(values, 2 * index + 2);
  }
  return node;
}

void BinaryTree::visit(const Node* node, Order order, std::vector<int>& result)
{
  if (!node)
  {
    return;
  }

  // Меняется только момент обработки корня относительно двух поддеревьев.
  if (order == Order::Pre)
  {
    result.push_back(node->key);
  }
  visit(node->left.get(), order, result);
  if (order == Order::In)
  {
    result.push_back(node->key);
  }
  visit(node->right.get(), order, result);
  if (order == Order::Post)
  {
    result.push_back(node->key);
  }
}

std::vector<int> BinaryTree::traverse(Order order) const
{
  std::vector<int> result;
  visit(root.get(), order, result);
  return result;
}

std::size_t BinaryTree::height(const Node* node)
{
  if (!node)
  {
    return 0;
  }
  // Текущий уровень добавляется к более длинному из двух путей вниз.
  return 1 + std::max(height(node->left.get()), height(node->right.get()));
}

void BinaryTree::print(const Node* node, std::ostream& out, std::size_t depth, char side)
{
  if (!node)
  {
    return;
  }
  // Дерево повёрнуто: правые дети выше, левые ниже родителя.
  print(node->right.get(), out, depth + 1, 'R');
  out << std::string(depth * 4, ' ') << side << ": " << node->key << '\n';
  print(node->left.get(), out, depth + 1, 'L');
}

void BinaryTree::print(std::ostream& out) const
{
  if (!root)
  {
    out << "(пусто)\n";
  }
  else
  {
    print(root.get(), out, 0, '*');
  }
}
