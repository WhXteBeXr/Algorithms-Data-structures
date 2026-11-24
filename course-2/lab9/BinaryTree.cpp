#include "BinaryTree.h"

#include <algorithm>
#include <ostream>
#include <string>

// Создаёт собственные узлы по массиву уровней. Значения копируются, сам span не сохраняется.
BinaryTree::BinaryTree(std::span<const int> values) : root(build(values, 0)) {}

// Строит поддерево с вершиной values[index] и возвращает единственного владельца его корня.
std::unique_ptr<BinaryTree::Node> BinaryTree::build(std::span<const int> values, std::size_t index)
{
  // Выход за массив означает отсутствие узла, в том числе корня для пустого входа.
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
  // Владение готовым поддеревом передаётся родительской связи или корню дерева.
  // При исключении в рекурсии unique_ptr освободит уже созданную часть автоматически.
  return node;
}

// Дописывает ключи поддерева в общий result в заданном порядке Pre, In или Post.
// node только наблюдает узел, владение им и связями остаётся у дерева.
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
  // get() передаёт невладеющий адрес, result по ссылке накапливает ответы всех вызовов.
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

// Создаёт отдельный результат обхода всего дерева. Состояние и связи дерева не меняются.
std::vector<int> BinaryTree::traverse(Order order) const
{
  std::vector<int> result;
  visit(root.get(), order, result);
  return result;
}

// Возвращает число уровней в поддереве. Для пустой связи высота 0, для листа 1.
std::size_t BinaryTree::height(const Node* node)
{
  if (!node)
  {
    return 0;
  }
  // Текущий уровень добавляется к более длинному из двух путей вниз.
  return 1 + std::max(height(node->left.get()), height(node->right.get()));
}

// Печатает поддерево боком. Depth задаёт отступ, side отмечает корень (*), левого или правого ребёнка.
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

// Запускает печать от корня с нулевым отступом или явно сообщает о пустом дереве.
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
