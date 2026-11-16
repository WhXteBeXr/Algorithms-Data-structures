#include "BST.h"

#include <stdexcept>
#include <utility>

bool BST::insert(int key)
{
  // Храним адрес владеющей связи, чтобы одинаково менять корень и ребёнка.
  Link* link = &tree.root;
  while (*link)
  {
    if ((*link)->key == key)
    {
      return false;
    }
    link = key < (*link)->key ? &(*link)->left : &(*link)->right;
  }
  // Только пустая связь получает новый узел. При ошибке выделения счётчик прежний.
  *link = std::make_unique<Node>(key);
  ++count;
  return true;
}

bool BST::search(int key) const
{
  const Node* node = tree.root.get();
  while (node)
  {
    if (key == node->key)
    {
      return true;
    }
    // Свойство BST исключает целое поддерево на каждом шаге.
    node = key < node->key ? node->left.get() : node->right.get();
  }
  return false;
}

const BST::Node* BST::extreme(bool right) const
{
  const Node* node = tree.root.get();
  if (!node)
  {
    throw std::underflow_error("У пустого BST нет минимума и максимума");
  }
  // Минимум лежит в крайнем левом узле, максимум в крайнем правом.
  while (right ? bool(node->right) : bool(node->left))
  {
    node = right ? node->right.get() : node->left.get();
  }
  return node;
}

const BST::Node* BST::select(const Node* node, std::size_t& remaining)
{
  if (!node)
  {
    return nullptr;
  }
  // Обратный симметричный обход посещает ключи от большего к меньшему.
  if (const Node* found = select(node->right.get(), remaining))
  {
    return found;
  }
  if (--remaining == 0)
  {
    return node;
  }
  return select(node->left.get(), remaining);
}

int BST::kthLargest(std::ptrdiff_t k) const
{
  if (k <= 0 || static_cast<std::size_t>(k) > count)
  {
    throw std::out_of_range("Номер k должен быть от 1 до размера BST");
  }
  // Общий счётчик передаётся по ссылке, найденный узел завершает все вызовы.
  auto remaining = static_cast<std::size_t>(k);
  return select(tree.root.get(), remaining)->key;
}

bool BST::remove(int key)
{
  Link* link = &tree.root;
  while (*link && (*link)->key != key)
  {
    link = key < (*link)->key ? &(*link)->left : &(*link)->right;
  }
  if (!*link)
  {
    return false;
  }

  if (!(*link)->left || !(*link)->right)
  {
    // Сначала отделяем узел. Его ребёнок остаётся жив до передачи новой связи.
    auto removed = std::move(*link);
    *link = removed->left ? std::move(removed->left) : std::move(removed->right);
    // removed освобождает только старый узел, переданное поддерево сохранено.
  }
  else
  {
    // Преемник - минимум справа. У него нет левого ребёнка.
    Link* successor = &(*link)->right;
    while ((*successor)->left)
    {
      successor = &(*successor)->left;
    }
    (*link)->key = (*successor)->key;
    // Убираем физический узел преемника, сохраняя его возможного правого ребёнка.
    auto removed = std::move(*successor);
    *successor = std::move(removed->right);
  }
  --count;
  return true;
}
