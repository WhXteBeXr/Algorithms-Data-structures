#include "Comparison.h"
#include "Recursion.h"
#include "Sorts.h"

#include <exception>
#include <iostream>
#include <string>

namespace
{
  template <typename Action>
  void showError(const char* label, Action action)
  {
    try
    {
      action();
    }
    catch (const std::exception& error)
    {
      std::cout << label << ": " << error.what() << '\n';
    }
  }

  void demonstrateRecursion()
  {
    std::cout << "=== Часть 1. Рекурсивные алгоритмы ===\n";
    // Совпадение результатов сравнивает реализации на базовых и граничных значениях.
    for (int n : {0, 1, 5, 20})
    {
      std::cout << n << "!: recursion = " << factorialRecursive(n) << ", loop = " << factorialIterative(n) << '\n';
    }
    for (int n : {0, 1, 10, 93})
    {
      std::cout << "F(" << n << "): memo recursion = " << fibonacciRecursive(n)
                << ", loop = " << fibonacciIterative(n) << '\n';
    }
    std::cout << "Строки ASCII, регистр и пробелы учитываются:\n";
    for (const char* text : {"", "a", "algorithm", "level", "Level"})
    {
      std::cout << '"' << text << "\": reverse = \"" << reverseString(text)
                << "\", palindrome = " << isPalindrome(text) << '\n';
    }
    // Неверные аргументы перехватываются, дальнейшие примеры продолжают работу.
    showError("Отрицательный факториал", [] { factorialRecursive(-1); });
    showError("21!", [] { factorialIterative(21); });
    showError("Отрицательный индекс Фибоначчи", [] { fibonacciIterative(-1); });
    showError("F(94)", [] { fibonacciRecursive(94); });
    showError("Текст UTF-8", [] { reverseString("тест"); });
    showError("Слишком длинная строка", [] { isPalindrome(std::string(4097, 'a')); });
  }

  void demonstrateSorts()
  {
    std::cout << "\n=== Часть 2. Простые сортировки ===\n";
    for (const auto& algorithm : sortingAlgorithms)
    {
      int values[] = {4, -1, 4, 0, 2, -3};
      algorithm.sort(std::span<int>{values});
      std::cout << algorithm.name << ':';
      for (int value : values)
      {
        std::cout << ' ' << value;
      }
      // Пустой диапазон и один элемент не требуют обменов и не выходят за границы.
      int single[] = {42};
      algorithm.sort(std::span<int>{single});
      algorithm.sort({});
      std::cout << ", single = " << single[0] << ", empty: OK\n";
    }
  }
}

int main()
{
  std::cout << std::boolalpha;
  try
  {
    demonstrateRecursion();
    demonstrateSorts();
    compareSorts(std::cout);
  }
  catch (const std::exception& error)
  {
    std::cerr << "Ошибка выполнения: " << error.what() << '\n';
    return 1;
  }
}
