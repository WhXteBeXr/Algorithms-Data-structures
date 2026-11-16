#include "BST.h"

#include <algorithm>
#include <array>
#include <exception>
#include <iostream>
#include <numeric>
#include <random>
#include <span>
#include <stdexcept>
#include <string_view>
#include <vector>

namespace
{
  void printValues(std::string_view label, std::span<const int> values)
  {
    std::cout << label;
    if (values.empty())
    {
      std::cout << "(пусто)";
    }
    for (int value : values)
    {
      std::cout << value << ' ';
    }
    std::cout << '\n';
  }

  template <typename Action>
  void showError(std::string_view label, Action action)
  {
    // Демонстрация ожидаемой ошибки продолжается после перехвата исключения.
    std::cout << label;
    try
    {
      action();
    }
    catch (const std::exception& error)
    {
      std::cout << error.what() << '\n';
    }
  }

  void demonstrateBinaryTree()
  {
    std::cout << "Часть 1. Бинарное дерево, вариант 3\n";
    // Форма задаётся массивом по уровням. Ключи намеренно не образуют BST.
    const std::array values{10, 5, 15, 3, 7, 20, 6};
    BinaryTree tree(values);
    tree.print(std::cout);
    printValues("preOrder:  ", tree.preOrder());
    printValues("inOrder:   ", tree.inOrder());
    printValues("postOrder: ", tree.postOrder());
    std::cout << "Высота (уровни): " << tree.height() << '\n';

    // Базовый случай рекурсии определяет высоту пустого дерева и листа.
    BinaryTree empty;
    const std::array one{42};
    BinaryTree leaf(one);
    printValues("Обход пустого дерева: ", empty.inOrder());
    std::cout << "Высоты пустого дерева и листа: " << empty.height()
              << ", " << leaf.height() << "\n\n";
  }

  void demonstrateBST()
  {
    std::cout << "Часть 2. BST, вариант 3\n";
    BST tree;
    for (int key : {50, 30, 70, 20, 40, 60, 80})
    {
      tree.insert(key);
    }
    tree.print(std::cout);
    printValues("inOrder: ", tree.inOrder());
    std::cout << "Высота (уровни): " << tree.height() << '\n';
    std::cout << "Поиск 40: " << tree.search(40) << ", поиск 100: " << tree.search(100)
              << "\nПовторная вставка 50: " << tree.insert(50)
              << ", размер: " << tree.size() << '\n';
    // Крайние номера должны совпасть с максимумом и минимумом.
    for (int k : {1, 3, 7})
    {
      std::cout << "k = " << k << ", k-й наибольший: " << tree.kthLargest(k) << '\n';
    }
    for (int k : {-1, 0, 8})
    {
      std::cout << "k = " << k << ", ";
      showError("Неверный k: ", [&] { tree.kthLargest(k); });
    }

    // Подготовим узел 60 с одним ребёнком и пройдём все случаи удаления.
    tree.insert(65);
    for (int key : {20, 60, 70, 50, 999})
    {
      std::cout << "Удаление " << key << ": " << tree.remove(key) << '\n';
      printValues("Осталось: ", tree.inOrder());
    }
    std::cout << "Итоговая форма, размер " << tree.size() << ":\n";
    tree.print(std::cout);

    // Удаление единственного корня обнуляет владеющую связь, затем возможна вставка.
    BST single;
    single.insert(42);
    std::cout << "Удаление единственного узла: " << single.remove(42)
              << ", повторное: " << single.remove(42) << ", высота: " << single.height() << '\n';
    single.print(std::cout);
    showError("Минимум пустого BST: ", [&] { single.minimum(); });
    showError("Максимум пустого BST: ", [&] { single.maximum(); });
    showError("k = 1 у пустого BST: ", [&] { single.kthLargest(1); });
    single.insert(-5);
    std::cout << "Повторное использование: минимум " << single.minimum()
              << ", максимум " << single.maximum() << '\n';

    // Упорядоченная вставка показывает отсутствие автоматической балансировки.
    BST chain;
    for (int key = 1; key <= 7; ++key)
    {
      chain.insert(key);
    }
    std::cout << "Высота цепочки из 7 ключей: " << chain.height() << "\n\n";
  }

  void demonstrateApplication()
  {
    std::cout << "Часть 3. Применение BST, N = 20\n";
    // Перемешиваем диапазон без повторов, чтобы дерево содержало ровно 20 узлов.
    std::vector<int> values(101);
    std::iota(values.begin(), values.end(), -50);
    std::mt19937 generator(2026);
    std::shuffle(values.begin(), values.end(), generator);
    values.resize(20);
    BST tree;
    for (int key : values)
    {
      tree.insert(key);
    }
    printValues("Случайная последовательность: ", values);
    printValues("inOrder: ", tree.inOrder());
    std::cout << "Размер: " << tree.size() << ", минимум: " << tree.minimum()
              << ", максимум: " << tree.maximum() << ", высота (уровни): " << tree.height()
              << "\nФорма до удаления (* - корень, L/R - сторона ребёнка):\n";
    tree.print(std::cout);

    // Ключ вводится пользователем, отсутствие ключа не считается ошибкой.
    std::cout << "Введите целый ключ для удаления: ";
    int key;
    if (!(std::cin >> key))
    {
      throw std::invalid_argument("Ожидался целый ключ в диапазоне int");
    }
    std::cout << "\nУдаление ключа " << key << ": " << tree.remove(key) << '\n';
    printValues("inOrder после удаления: ", tree.inOrder());
    std::cout << "Размер: " << tree.size() << ", высота (уровни): " << tree.height()
              << "\nФорма после удаления:\n";
    tree.print(std::cout);
  }
} // namespace

int main()
{
  std::cout << std::boolalpha;
  try
  {
    demonstrateBinaryTree();
    demonstrateBST();
    demonstrateApplication();
  }
  catch (const std::exception& error)
  {
    std::cerr << "Ошибка: " << error.what() << '\n';
    return 1;
  }
}
