#include "Comparison.h"
#include "HeapSort.h"

#include <exception>
#include <iostream>
#include <limits>
#include <vector>

namespace
{
  void printValues(const char* label, std::span<const int> values)
  {
    std::cout << label << '[';
    for (std::size_t i = 0; i < values.size(); ++i)
    {
      if (i != 0)
      {
        std::cout << ", ";
      }
      std::cout << values[i];
    }
    std::cout << "]\n";
  }
}

int main()
{
  try
  {
    std::cout << "=== Средний уровень, вариант 3: min-heap ===\n";
    int example[] = {4, 10, 3, 5, 1};
    printValues("Исходный: ", example);
    buildMinHeap(example);
    printValues("После построения min-heap: ", example);
    std::cout << "Минимум в корне: " << example[0] << '\n';
    heapSortDescending(example);
    printValues("По убыванию: ", example);

    std::cout << "\n=== Граничные случаи ===\n";
    // Все допустимые размеры, дубликаты, знаки и обе границы int.
    const std::vector<int> samples[] = {
        {}, {42}, {7, 7, 7, 7}, {-3, 2, -3, 0, -1}, {1, 2, 3, 4}, {4, 3, 2, 1},
        {std::numeric_limits<int>::min(), 0, std::numeric_limits<int>::max()}};
    for (auto values : samples)
    {
      printValues("До:    ", values);
      heapSortDescending(values);
      printValues("После: ", values);
    }
    compareInputs(std::cout);
  }
  catch (const std::exception& error)
  {
    std::cerr << "Ошибка выполнения: " << error.what() << '\n';
    return 1;
  }
}
