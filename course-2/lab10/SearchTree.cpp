#include "SearchTree.h"

#include <algorithm>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

int SearchTree::cachedHeight(const Link& node)
{
  return node ? node->height : 0;
}

int SearchTree::balanceFactor(const Node& node)
{
  return cachedHeight(node.left) - cachedHeight(node.right);
}

void SearchTree::updateHeight(Node& node)
{
  node.height = 1 + std::max(cachedHeight(node.left), cachedHeight(node.right));
}

void SearchTree::rotateRight(Link& node)
{
  // Сохраняем владельца левого ребёнка, который станет новым корнем поддерева.
  auto top = std::move(node->left);
  node->left = std::move(top->right); // Среднее поддерево остаётся между двумя ключами.
  updateHeight(*node);
  top->right = std::move(node);
  // Сначала обновлена высота опустившегося узла, теперь нового корня.
  updateHeight(*top);
  node = std::move(top);
}

void SearchTree::rotateLeft(Link& node)
{
  // Зеркальная передача связей, узлы не копируются и не освобождаются.
  auto top = std::move(node->right);
  node->right = std::move(top->left);
  updateHeight(*node);
  top->left = std::move(node);
  updateHeight(*top);
  node = std::move(top);
}

void SearchTree::balance(Link& node)
{
  updateHeight(*node);
  const int factor = balanceFactor(*node);
  if (factor > 1)
  {
    // LR сначала превращается в LL поворотом левого ребёнка.
    if (balanceFactor(*node->left) < 0)
    {
      rotateLeft(node->left);
    }
    rotateRight(node);
  }
  else if (factor < -1)
  {
    // RL сначала превращается в RR поворотом правого ребёнка.
    if (balanceFactor(*node->right) > 0)
    {
      rotateRight(node->right);
    }
    rotateLeft(node);
  }
}

bool SearchTree::insertAVL(Link& node, int key)
{
  if (!node)
  {
    node = std::make_unique<Node>(key);
    return true;
  }
  if (key == node->key)
  {
    return false;
  }
  Link& child = key < node->key ? node->left : node->right;
  const bool inserted = insertAVL(child, key);
  if (inserted)
  {
    // При возврате из рекурсии дети уже сбалансированы, исправляем их родителя.
    balance(node);
  }
  return inserted;
}

bool SearchTree::insert(int key)
{
  if (kind == Kind::AVL)
  {
    return insertAVL(root, key);
  }
  // У обычного BST нет поворотов. Цикл безопасен и для цепочки из 10000 узлов.
  Link* link = &root;
  while (*link)
  {
    if (key == (*link)->key)
    {
      return false;
    }
    link = key < (*link)->key ? &(*link)->left : &(*link)->right;
  }
  *link = std::make_unique<Node>(key);
  return true;
}

bool SearchTree::search(int key) const
{
  const Node* node = root.get();
  while (node)
  {
    if (key == node->key)
    {
      return true;
    }
    // Общий поиск одинаков для BST и AVL, отличается только длина пути.
    node = key < node->key ? node->left.get() : node->right.get();
  }
  return false;
}

int SearchTree::height() const
{
  if (kind == Kind::AVL)
  {
    return cachedHeight(root);
  }
  // BST не обновляет кеш. Измеряем его форму отдельно от замеров времени.
  int result = 0;
  std::vector<std::pair<const Node*, int>> pending;
  if (root)
  {
    pending.emplace_back(root.get(), 1);
  }
  while (!pending.empty())
  {
    const auto [node, depth] = pending.back();
    pending.pop_back();
    result = std::max(result, depth);
    if (node->left)
    {
      pending.emplace_back(node->left.get(), depth + 1);
    }
    if (node->right)
    {
      pending.emplace_back(node->right.get(), depth + 1);
    }
  }
  return result;
}

void SearchTree::print(const Node* node, std::ostream& out, int depth, char side, bool showBalance)
{
  if (!node)
  {
    return;
  }
  // Правое поддерево выше корня, левое ниже. Отступ отражает глубину.
  print(node->right.get(), out, depth + 1, 'R', showBalance);
  out << std::string(depth * 4, ' ') << side << ": " << node->key;
  if (showBalance)
  {
    out << " (h=" << node->height << ", bf=" << balanceFactor(*node) << ')';
  }
  out << '\n';
  print(node->left.get(), out, depth + 1, 'L', showBalance);
}

void SearchTree::print(std::ostream& out) const
{
  if (!root)
  {
    out << "(пусто)\n";
  }
  else
  {
    print(root.get(), out, 0, '*', kind == Kind::AVL);
  }
}

SearchTree::~SearchTree()
{
  // Рекурсивное разрушение глубокой BST-цепочки заменяем циклом без выделений.
  while (root)
  {
    if (root->left)
    {
      // Правый поворот постепенно освобождает левую сторону корня.
      rotateRight(root);
    }
    else
    {
      auto removed = std::move(root);
      root = std::move(removed->right);
      // У removed больше нет детей, его уничтожение не запускает длинную рекурсию.
    }
  }
}
