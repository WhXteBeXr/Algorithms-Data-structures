#include "StringAlgorithms.h"

#include <exception>
#include <iomanip>
#include <iostream>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>

namespace
{
  void printNumbers(std::span<const std::size_t> numbers)
  {
    if (numbers.empty())
    {
      std::cout << "(пусто)";
    }
    for (auto number : numbers)
    {
      std::cout << number << ' ';
    }
    std::cout << '\n';
  }

  void printSearch(std::string_view text, std::string_view pattern, bool showPrefix = false)
  {
    std::cout << "Текст=" << std::quoted(text) << ", образец=" << std::quoted(pattern) << '\n';
    if (showPrefix)
    {
      std::cout << "Префикс-функция: ";
      printNumbers(prefixFunction(pattern));
    }
    const auto positions = kmpSearch(text, pattern);
    std::cout << "Позиции (с нуля): ";
    printNumbers(positions);
    std::cout << "Количество: " << positions.size() << '\n';
  }

  void printAnagram(std::string_view first, std::string_view second)
  {
    std::cout << "Анаграмма " << std::quoted(first) << " и " << std::quoted(second)
              << ": " << areAnagrams(first, second) << '\n';
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
    std::cout << "Часть 1. КМП, вариант 3\nПрефикс-функция ababaca: ";
    printNumbers(prefixFunction("ababaca"));
    const std::string_view text = "ababaababcabab";
    const std::string_view pattern = "abab";
    printSearch(text, pattern, true);
    // Выравнивание печатается только для маленького демонстрационного примера.
    std::cout << "Визуализация вхождений:\n" << text << '\n';
    for (auto position : kmpSearch(text, pattern))
    {
      std::cout << std::string(position, ' ') << pattern << '\n';
    }

    std::cout << "\nЧасть 2. Анаграммы через string и sort, вариант 3\n";
    printAnagram("listen", "silent");
    printAnagram("aab", "aba"); // Учитывается не только наличие букв, но и число повторений.
    printAnagram("aab", "abb");
    printAnagram("Listen", "silent"); // Изменение регистра не выполняется.
    printAnagram("ab ", "ba"); // Пробел не удаляется автоматически.
    printAnagram("", "");
  }

  void demonstrateBoundaries()
  {
    std::cout << "\nГраничные случаи\n";
    printSearch("aaaaa", "aaa"); // Все три перекрывающихся вхождения должны сохраниться.
    printSearch("abc", "xyz");
    printSearch("abc", "abcd");
    printSearch("", "a");
    printSearch("abc", "");
    printSearch("", "");
    printSearch("abc", "abc");
    printSearch("bbb", "b");
    std::cout << "Префикс-функция пустой строки: ";
    printNumbers(prefixFunction(""));
    std::cout << "Префикс-функция одного символа: ";
    printNumbers(prefixFunction("a"));
    showError("Неподдерживаемый алфавит", [] { kmpSearch("текст", "е"); });
  }

  void runInteractive()
  {
    std::cout << "\nЧасть 3. Пользовательский текст и образец\n"
              << "Учитываются регистр и пробелы, допустим печатный ASCII.\nТекст: ";
    std::string text;
    std::string pattern;
    // getline сохраняет пробелы и допускает пустую строку, лишняя нормализация не нужна.
    if (!std::getline(std::cin, text))
    {
      throw std::invalid_argument("Не удалось прочитать текст");
    }
    std::cout << "Образец: ";
    if (!std::getline(std::cin, pattern))
    {
      throw std::invalid_argument("Не удалось прочитать образец");
    }
    std::cout << '\n';
    printSearch(text, pattern, true);
    // Дополнительная обработка варианта 3 применяется к тем же двум введённым строкам.
    printAnagram(text, pattern);
  }
}

int main()
{
  std::cout << std::boolalpha;
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
