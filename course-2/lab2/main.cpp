#include "Benchmark.h"
#include "Stack.h"

#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>

namespace
{
  void printComplexity()
  {
    std::cout << "=== Сложность операций Stack<T> ===\n"
              << "push: O(1) амортизированно, O(n) при расширении\n"
              << "pop: O(1), top: O(1), isEmpty: O(1), size: O(1)\n"
              << "Оценки даны при постоянной стоимости операций над T.\n\n";
  }

  void demonstrateIntegers()
  {
    // Маленькая ёмкость заставляет стек расширяться уже в коротком примере.
    Stack<int> numbers(2);
    std::cout << "=== Stack<int>: рост и порядок LIFO ===\n";
    std::cout << "До добавления: размер = " << numbers.size() << ", пуст = " << numbers.isEmpty() << '\n';
    for (int value : {10, 20, 30, 40, 50})
    {
      numbers.push(value);
      std::cout << "push(" << value << "): размер = " << numbers.size() << ", top = " << numbers.top() << '\n';
    }

    const Stack<int>& view = numbers;
    std::cout << "top через const-ссылку: " << view.top() << ", размер = " << view.size() << '\n';
    std::cout << "Извлечение в обратном порядке:";
    while (!numbers.isEmpty())
    {
      std::cout << ' ' << numbers.pop();
    }
    std::cout << "\nПосле извлечения: размер = " << numbers.size() << ", пуст = " << numbers.isEmpty() << '\n';
    numbers.push(99); // Освободившиеся ячейки можно использовать повторно.
    std::cout << "Повторное использование: pop = " << numbers.pop() << '\n';
  }

  void demonstrateOtherTypes()
  {
    std::cout << "\n=== Другие типы данных ===\n";
    Stack<std::string> words(1);
    words.push("один");
    // Вершина находится в старом буфере. push сохраняет её до завершения расширения.
    words.push(words.top());
    std::cout << "push(top()) с расширением: top = " << words.top() << ", размер = " << words.size() << '\n';
    words.push("два");
    const auto lastWord = words.pop();
    std::cout << "Stack<string>: pop = " << lastWord << ", следующая вершина = " << words.top() << '\n';

    Stack<double> fractions;
    fractions.push(1.5);
    fractions.push(2.75);
    std::cout << "Stack<double>: top = " << fractions.top();
    const double lastFraction = fractions.pop();
    std::cout << ", pop = " << lastFraction << ", размер = " << fractions.size() << '\n';
  }

  void demonstrateBoundaries()
  {
    std::cout << "\n=== Граничные случаи ===\n";
    Stack<int> empty;
    // Ошибки перехватываются отдельно, после каждой программа продолжает работу.
    try
    {
      empty.pop();
    }
    catch (const std::out_of_range& error)
    {
      std::cout << "pop пустого стека: " << error.what() << '\n';
    }
    try
    {
      empty.top();
    }
    catch (const std::out_of_range& error)
    {
      std::cout << "top пустого стека: " << error.what() << '\n';
    }
    std::cout << "После ошибок: размер = " << empty.size() << ", пуст = " << empty.isEmpty() << '\n';

    Stack<int> tiny(0); // Нулевая ёмкость исправляется на единицу.
    tiny.push(1);
    tiny.push(2);
    std::cout << "Начальная ёмкость 0, два push: размер = " << tiny.size() << ", top = " << tiny.top() << '\n';
  }
}

int main()
{
  std::cout << std::boolalpha;
  try
  {
    printComplexity();
    demonstrateIntegers();
    demonstrateOtherTypes();
    demonstrateBoundaries();
    // Замеры отделены от демонстрационного вывода, чтобы печать не влияла на время.
    runStackBenchmark(std::cout);
  }
  catch (const std::exception& error)
  {
    std::cerr << "Ошибка выполнения: " << error.what() << '\n';
    return 1;
  }
}
