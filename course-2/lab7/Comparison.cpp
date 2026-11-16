#include "Comparison.h"
#include "HeapSort.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <functional>
#include <iomanip>
#include <ostream>
#include <random>
#include <stdexcept>
#include <string_view>
#include <vector>

void compareInputs(std::ostream& out)
{
  out << "\n=== Время сортировки по убыванию ===\n"
      << "Микросекунды, медиана 5 запусков. Для сравнения используйте Release.\n"
      << std::setw(8) << "N" << std::setw(14) << "Input" << std::setw(16) << "time_us" << '\n';
  std::mt19937 generator(2026); // Фиксированный seed делает входы воспроизводимыми.
  for (int count : {100, 1000, 10000})
  {
    std::uniform_int_distribution<int> randomValue(-count, count);
    for (std::string_view kind : {"Random", "Ascending", "Descending", "Equal"})
    {
      std::vector<int> original(count);
      for (int i = 0; i < count; ++i)
      {
        if (kind == "Random")
        {
          original[i] = randomValue(generator);
        }
        else if (kind == "Ascending")
        {
          original[i] = i;
        }
        else if (kind == "Descending")
        {
          original[i] = count - i;
        }
        else
        {
          original[i] = 7;
        }
      }
      auto expected = original;
      std::sort(expected.begin(), expected.end(), std::greater<int>{});
      std::array<double, 5> times{};
      for (double& time : times)
      {
        auto values = original;
        const auto start = std::chrono::steady_clock::now();
        heapSortDescending(values);
        const auto finish = std::chrono::steady_clock::now();
        time = std::chrono::duration<double, std::micro>(finish - start).count();
        // Эталон нужен только для проверки полного результата, не для замера STL.
        if (values != expected)
        {
          throw std::logic_error("Пирамидальная сортировка дала неверный результат");
        }
      }
      std::sort(times.begin(), times.end());
      out << std::fixed << std::setprecision(2) << std::setw(8) << count << std::setw(14) << kind
          << std::setw(16) << times[times.size() / 2] << '\n';
    }
  }
  out << "Копирование, эталон std::sort и проверка результата находятся вне замера.\n";
}
