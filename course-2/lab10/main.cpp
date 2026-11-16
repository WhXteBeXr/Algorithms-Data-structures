#include "Comparison.h"
#include "SearchTree.h"
#include "Trie.h"

#include <array>
#include <exception>
#include <iomanip>
#include <iostream>
#include <limits>
#include <span>
#include <string>

namespace
{
  void printWords(std::span<const std::string> words)
  {
    if (words.empty())
    {
      std::cout << "(пусто)";
    }
    for (const auto& word : words)
    {
      std::cout << std::quoted(word) << ' ';
    }
    std::cout << '\n';
  }

  void demonstrateAVL()
  {
    std::cout << "Часть 1. AVL, вариант 3\n";
    // Четыре коротких последовательности отдельно показывают каждый тип исправления.
    const std::array<std::array<int, 3>, 4> cases{{{30, 20, 10}, {10, 20, 30}, {30, 10, 20}, {10, 30, 20}}};
    const std::array names{"LL: правый поворот", "RR: левый поворот", "LR: левый-правый", "RL: правый-левый"};
    for (std::size_t i = 0; i < cases.size(); ++i)
    {
      SearchTree tree(SearchTree::Kind::AVL);
      std::cout << names[i] << ", вставки: ";
      for (int key : cases[i])
      {
        std::cout << key << ' ';
        tree.insert(key);
      }
      std::cout << '\n';
      tree.print(std::cout); // Во всех четырёх случаях корень 20, высота 2.
    }

    // Более длинный пример показывает кеш высот и поиск после нескольких поворотов.
    SearchTree tree(SearchTree::Kind::AVL);
    for (int key : {10, 20, 30, 40, 50, 25})
    {
      tree.insert(key);
    }
    std::cout << "Вставки 10 20 30 40 50 25, итоговая форма:\n";
    tree.print(std::cout);
    std::cout << "Высота (уровни): " << tree.height()
              << ", поиск 25: " << tree.search(25) << ", поиск 26: " << tree.search(26)
              << "\nДубликат 25: " << tree.insert(25) << '\n';

    // Пустая структура и крайние int не требуют арифметики над самими ключами.
    SearchTree empty(SearchTree::Kind::AVL);
    std::cout << "Пустое AVL: высота " << empty.height() << ", поиск 0: " << empty.search(0) << '\n';
    empty.print(std::cout);
    empty.insert(std::numeric_limits<int>::min());
    empty.insert(std::numeric_limits<int>::max());
    std::cout << "Поиск INT_MIN и INT_MAX: " << empty.search(std::numeric_limits<int>::min())
              << ", " << empty.search(std::numeric_limits<int>::max()) << "\n\n";
  }

  void demonstrateTrie()
  {
    std::cout << "Часть 2. Trie, вариант 3\n";
    Trie trie;
    for (const char* word : {"cat", "car", "dog", "cart", "catfish", "app", "apple"})
    {
      trie.insert(word);
    }
    std::cout << "Все слова: ";
    printWords(trie.getAllWords());
    std::cout << "Повторная вставка cat: " << trie.insert("cat") << '\n';
    // Путь ca существует, но маркер конца стоит у car и cat, а не у ca.
    for (const char* word : {"cat", "car", "ca", "cattle", "app", "apple"})
    {
      std::cout << "search(" << std::quoted(word) << "): " << trie.search(word) << '\n';
    }
    for (const char* prefix : {"ca", "catf", "do", "ba", ""})
    {
      std::cout << "startsWith(" << std::quoted(prefix) << "): " << trie.startsWith(prefix) << '\n';
    }
    // Неверная строка проверяется целиком, список слов после ошибки остаётся прежним.
    try
    {
      trie.insert("car2");
    }
    catch (const std::exception& error)
    {
      std::cout << "Вставка car2: " << error.what() << '\n';
    }
    std::cout << "Слова после неверного ввода: ";
    printWords(trie.getAllWords());

    Trie empty;
    std::cout << "Пустой Trie, все слова: ";
    printWords(empty.getAllWords());
    std::cout << "search(cat): " << empty.search("cat") << ", startsWith(c): " << empty.startsWith("c")
              << ", startsWith(пустой): " << empty.startsWith("") << '\n';
    // Пустое слово отличается от пустого набора маркером на корне.
    std::cout << "Вставка пустого слова: " << empty.insert("") << ", search(пустой): " << empty.search("")
              << ", повторная вставка: " << empty.insert("") << '\n';
    std::cout << "Список с пустым словом: ";
    printWords(empty.getAllWords());
    std::cout << '\n';
  }
}

int main()
{
  std::cout << std::boolalpha;
  try
  {
    demonstrateAVL();
    demonstrateTrie();
    runComparison(std::cout);
  }
  catch (const std::exception& error)
  {
    std::cerr << "Ошибка: " << error.what() << '\n';
    return 1;
  }
}
