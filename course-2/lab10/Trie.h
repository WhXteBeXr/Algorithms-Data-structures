#pragma once

#include <array>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

class Trie
{
public:
  Trie() = default;
  Trie(const Trie&) = delete;
  Trie& operator=(const Trie&) = delete;

  // Алфавит a-z. Пустое слово допустимо, дубликат даёт false.
  bool insert(std::string_view word);
  bool search(std::string_view word) const;
  bool startsWith(std::string_view prefix) const;
  std::vector<std::string> getAllWords() const;

private:
  struct Node
  {
    std::array<std::unique_ptr<Node>, 26> children{};
    bool terminal = false;
  };
  Node root; // Корень соответствует пустому префиксу и существует всегда.

  static void validate(std::string_view text);
  const Node* findNode(std::string_view text) const;
  static void collect(const Node& node, std::string& word, std::vector<std::string>& result);
};
