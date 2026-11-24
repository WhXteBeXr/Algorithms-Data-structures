#include "BST.h"

#include <stdexcept>
#include <utility>

// Вставляет новый ключ в BST. Возвращает false для дубликата, true при создании узла.
bool BST::insert(int key)
{
  // Храним адрес владеющей связи, чтобы одинаково менять корень и ребёнка.
  Link* link = &tree.root;
  while (*link)
  {
    // Дубликаты не храним, поэтому не меняем ни связи, ни счётчик.
    if ((*link)->key == key)
    {
      return false;
    }
    // Передвигаем адрес связи, не перемещая владеющий unique_ptr из самого дерева.
    link = key < (*link)->key ? &(*link)->left : &(*link)->right;
  }
  // Только пустая связь получает новый узел. При ошибке выделения счётчик прежний.
  *link = std::make_unique<Node>(key);
  ++count;
  return true;
}

// Проверяет наличие ключа за O(h), где h - высота дерева. Узлы и связи не изменяет.
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
  // Дошли до отсутствующего ребёнка, значит ключа на единственном подходящем пути нет.
  return false;
}

// Возвращает невладеющий адрес минимума при right == false или максимума при true.
// В пустом дереве значения нет, поэтому вместо указателя бросается underflow_error.
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

// Ищет узел нужного ранга в поддереве. Remaining - сколько узлов ещё нужно посетить.
// Счётчик общий для всех вызовов, nullptr означает, что в этом поддереве ответ ещё не найден.
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
  // Правая часть уже посещена, теперь учитываем текущий узел как следующий по убыванию.
  if (--remaining == 0)
  {
    return node;
  }
  // Ответ пока не найден, продолжаем среди меньших ключей с оставшимся рангом.
  return select(node->left.get(), remaining);
}

// Возвращает k-й наибольший ключ, нумерация начинается с 1. Неверный k даёт out_of_range.
int BST::kthLargest(std::ptrdiff_t k) const
{
  if (k <= 0 || static_cast<std::size_t>(k) > count)
  {
    throw std::out_of_range("Номер k должен быть от 1 до размера BST");
  }
  // Общий счётчик передаётся по ссылке, найденный узел завершает все вызовы.
  auto remaining = static_cast<std::size_t>(k);
  // Проверка k и счётчик count гарантируют, что select найдёт существующий узел.
  return select(tree.root.get(), remaining)->key;
}

// Удаляет ключ за O(h). False означает отсутствие, при успехе число узлов уменьшается на один.
bool BST::remove(int key)
{
  // Ищем адрес владельца удаляемого узла: это root либо одна из связей left/right родителя.
  Link* link = &tree.root;
  while (*link && (*link)->key != key)
  {
    link = key < (*link)->key ? &(*link)->left : &(*link)->right;
  }
  // Ключ не найден, структура и count остаются прежними.
  if (!*link)
  {
    return false;
  }

  // Случаи листа и одного ребёнка одинаковы: родитель получает оставшееся поддерево или nullptr.
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
    // Копируем только ключ. Сам узел с двумя детьми и обе его связи пока остаются на месте.
    (*link)->key = (*successor)->key;
    // Убираем физический узел преемника, сохраняя его возможного правого ребёнка.
    auto removed = std::move(*successor);
    *successor = std::move(removed->right);
  }
  // В обеих ветках физически исчез ровно один узел, счётчик обновляем в общем месте.
  --count;
  return true;
}
