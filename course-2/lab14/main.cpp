#include "Knapsack.h"
#include "Lcs.h"

#include <algorithm>
#include <array>
#include <exception>
#include <iomanip>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace
{
  void printKnapsack(std::span<const Item> items, int capacity, const KnapsackResult& result)
  {
    std::cout << "Предметов " << items.size() << ", вместимость " << capacity << '\n';
    for (std::size_t i = 0; i < items.size(); ++i)
    {
      std::cout << i << ": вес=" << items[i].weight << ", ценность=" << items[i].value << '\n';
    }
    std::cout << "Максимальная ценность: " << result.maxValue << "\nВыбранные индексы:";
    int totalWeight = 0;
    for (auto index : result.selected)
    {
      std::cout << ' ' << index;
      totalWeight += items[index].weight;
    }
    if (result.selected.empty())
    {
      std::cout << " (пусто)";
    }
    std::cout << "\nСуммарный вес: " << totalWeight << '\n';
  }

  void printLcs(std::string_view first, std::string_view second, const LcsResult& result)
  {
    std::cout << "X=" << std::quoted(first) << ", Y=" << std::quoted(second)
              << "\nДлина LCS: " << result.length << ", различных вариантов: " << result.sequences.size() << '\n';
    for (const auto& sequence : result.sequences)
    {
      std::cout << std::quoted(sequence) << '\n';
    }
  }

  void printKnapsackTable(const KnapsackResult& result)
  {
    // Ширина учитывает самую большую ценность и последний номер столбца.
    const int width = static_cast<int>(std::max(std::to_string(result.maxValue).size(),
      std::to_string(result.table.front().size() - 1).size())) + 2;
    std::cout << "Таблица рюкзака: строки - первые i предметов, столбцы - вместимость w\n i/w";
    for (std::size_t w = 0; w < result.table.front().size(); ++w)
    {
      std::cout << std::setw(width) << w;
    }
    std::cout << '\n';
    for (std::size_t i = 0; i < result.table.size(); ++i)
    {
      std::cout << std::setw(4) << i;
      for (auto value : result.table[i])
      {
        std::cout << std::setw(width) << value;
      }
      std::cout << '\n';
    }
  }

  void printLcsTable(std::string_view first, std::string_view second, const LcsResult& result)
  {
    const int width = static_cast<int>(std::to_string(result.length).size()) + 2;
    std::cout << "Таблица LCS: строки - префиксы X, столбцы - префиксы Y\n     " << std::setw(width) << '-';
    for (char symbol : second)
    {
      std::cout << std::setw(width) << symbol;
    }
    std::cout << '\n';
    for (std::size_t i = 0; i < result.table.size(); ++i)
    {
      std::cout << std::setw(5) << (i == 0 ? '-' : first[i - 1]);
      for (auto value : result.table[i])
      {
        std::cout << std::setw(width) << value;
      }
      std::cout << '\n';
    }
  }

  template <typename Action>
  void showError(std::string_view label, Action action)
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

  void demonstrateExamples()
  {
    std::cout << "Часть 1. Рюкзак 0/1, вариант 3\n";
    const std::array<Item, 4> items{{{2, 3}, {3, 4}, {4, 5}, {5, 8}}};
    printKnapsack(items, 10, solveKnapsack(items, 10));
    std::cout << "\nЧасть 2. Все LCS, вариант 3\n";
    printLcs("ABCBDAB", "BDCABA", findAllLcs("ABCBDAB", "BDCABA"));
  }

  void demonstrateBoundaries()
  {
    std::cout << "\nГраничные случаи\n";
    printKnapsack({}, 3, solveKnapsack({}, 3));
    const std::array<Item, 3> zero{{{0, 5}, {0, 3}, {2, 10}}};
    printKnapsack(zero, 0, solveKnapsack(zero, 0)); // Даже при W=0 можно взять первые два предмета.
    const std::array<Item, 1> single{{{2, 3}}};
    printKnapsack(single, 4, solveKnapsack(single, 4)); // Ценность 3, а не 6: повтор запрещён.
    const std::array<Item, 2> negative{{{1, -7}, {2, -2}}};
    printKnapsack(negative, 3, solveKnapsack(negative, 3)); // Пустой набор выгоднее отрицательных ценностей.
    const std::array<Item, 2> large{{{1, std::numeric_limits<int>::max()}, {1, std::numeric_limits<int>::max()}}};
    std::cout << "Два INT_MAX, максимум: " << solveKnapsack(large, 2).maxValue << '\n';
    showError("Отрицательная вместимость", [&] { solveKnapsack(single, -1); });
    const std::array<Item, 1> invalid{{{-1, 5}}};
    showError("Отрицательный вес", [&] { solveKnapsack(invalid, 3); });

    // Пустая LCS, разные оптимумы и одинаковые строки с разными выравниваниями.
    printLcs("", "ABC", findAllLcs("", "ABC"));
    printLcs("ABC", "XYZ", findAllLcs("ABC", "XYZ"));
    printLcs("AAA", "AA", findAllLcs("AAA", "AA"));
    printLcs("ab", "ba", findAllLcs("ab", "ba"));
    showError("Неподдерживаемая кодировка", [] { findAllLcs("Я", "Я"); });
  }

  void runInteractive()
  {
    std::cout << "\nЧасть 3. Пользовательские данные и таблицы ДП\nКоличество предметов: ";
    int count;
    if (!(std::cin >> count) || count < 0)
    {
      throw std::invalid_argument("Ожидалось неотрицательное число предметов в диапазоне int");
    }
    std::cout << "Вместимость рюкзака: ";
    int capacity;
    if (!(std::cin >> capacity) || capacity < 0)
    {
      throw std::invalid_argument("Ожидалась неотрицательная вместимость в диапазоне int");
    }
    std::vector<Item> items(static_cast<std::size_t>(count));
    for (std::size_t i = 0; i < items.size(); ++i)
    {
      std::cout << "Предмет " << i << " (вес и ценность): ";
      if (!(std::cin >> items[i].weight >> items[i].value) || items[i].weight < 0)
      {
        throw std::invalid_argument("Нужны целые вес и ценность, вес неотрицательный");
      }
    }
    std::cout << '\n';
    const auto knapsack = solveKnapsack(items, capacity);
    printKnapsack(items, capacity, knapsack);
    printKnapsackTable(knapsack);

    // Убираем остаток числовой строки, затем читаем строки целиком, сохраняя пробелы.
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string first;
    std::string second;
    std::cout << "Строка X (печатные ASCII): ";
    if (!std::getline(std::cin, first))
    {
      throw std::invalid_argument("Не удалось прочитать строку X");
    }
    std::cout << "Строка Y (печатные ASCII): ";
    if (!std::getline(std::cin, second))
    {
      throw std::invalid_argument("Не удалось прочитать строку Y");
    }
    std::cout << '\n';
    const auto lcs = findAllLcs(first, second);
    printLcs(first, second, lcs);
    printLcsTable(first, second, lcs);
  }
}

int main()
{
  try
  {
    demonstrateExamples();
    demonstrateBoundaries();
    runInteractive();
  }
  catch (const std::exception& error)
  {
    std::cerr << "Ошибка: " << error.what() << '\n';
    return 1;
  }
}
