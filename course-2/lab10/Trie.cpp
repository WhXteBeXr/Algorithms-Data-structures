#include "Trie.h"

#include <stdexcept>

void Trie::validate(std::string_view text)
{
  // Проверяем всю строку до вставки, чтобы неверный символ не оставил часть пути.
  for (char ch : text)
  {
    if (ch < 'a' || ch > 'z')
    {
      throw std::invalid_argument("Trie поддерживает только буквы a-z");
    }
  }
}

bool Trie::insert(std::string_view word)
{
  validate(word);
  Node* node = &root;
  for (char ch : word)
  {
    auto& child = node->children[ch - 'a'];
    if (!child)
    {
      child = std::make_unique<Node>();
    }
    node = child.get();
  }
  // Путь может быть общим с длинным словом, отдельный маркер отличает целое слово.
  const bool inserted = !node->terminal;
  node->terminal = true;
  return inserted;
}

const Trie::Node* Trie::findNode(std::string_view text) const
{
  validate(text);
  const Node* node = &root;
  for (char ch : text)
  {
    node = node->children[ch - 'a'].get();
    if (!node)
    {
      return nullptr;
    }
  }
  return node;
}

bool Trie::search(std::string_view word) const
{
  const Node* node = findNode(word);
  return node && node->terminal;
}

bool Trie::startsWith(std::string_view prefix) const
{
  // Пустой префикс ведёт в корень даже у пустого Trie.
  return findNode(prefix) != nullptr;
}

void Trie::collect(const Node& node, std::string& word, std::vector<std::string>& result)
{
  if (node.terminal)
  {
    result.push_back(word);
  }
  // Переходы a-z дают лексикографический порядок без дополнительной сортировки.
  for (std::size_t i = 0; i < node.children.size(); ++i)
  {
    if (node.children[i])
    {
      word.push_back(static_cast<char>('a' + i));
      collect(*node.children[i], word, result);
      word.pop_back(); // Возвращаем общий буфер к префиксу родителя перед соседней ветвью.
    }
  }
}

std::vector<std::string> Trie::getAllWords() const
{
  std::vector<std::string> result;
  std::string word;
  collect(root, word, result);
  return result;
}
