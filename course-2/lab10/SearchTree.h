#pragma once

#include <iosfwd>
#include <memory>

class SearchTree
{
public:
  enum class Kind { BST, AVL };
  explicit SearchTree(Kind kind) : kind(kind) {}
  ~SearchTree();
  SearchTree(const SearchTree&) = delete;
  SearchTree& operator=(const SearchTree&) = delete;

  // В обоих режимах дубликаты игнорируются, true означает новый узел.
  bool insert(int key);
  bool search(int key) const;
  int height() const;
  void print(std::ostream& out) const;

private:
  struct Node
  {
    int key;
    int height = 1; // Кеш высоты поддерживается только в режиме AVL.
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
    explicit Node(int key) : key(key) {}
  };
  using Link = std::unique_ptr<Node>;
  Link root;
  Kind kind;

  static int cachedHeight(const Link& node);
  static int balanceFactor(const Node& node);
  static void updateHeight(Node& node);
  static void rotateRight(Link& node);
  static void rotateLeft(Link& node);
  static void balance(Link& node);
  static bool insertAVL(Link& node, int key);
  static void print(const Node* node, std::ostream& out, int depth, char side, bool showBalance);
};
