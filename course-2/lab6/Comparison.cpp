#include "Comparison.h"
#include "Sorts.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <iomanip>
#include <ostream>
#include <stdexcept>
#include <vector>

void compareSorts(std::ostream& out)
{
  out << "\n=== Сравнение сортировок на обратном массиве ===\n"
      << "Время в мкс, медиана 5 запусков. Для сравнения используйте Release.\n"
      << std::setw(8) << "N" << std::setw(14) << "Algorithm" << std::setw(16) << "time_us" << '\n';
  for (int count : {100, 1000, 5000})
  {
    std::vector<int> original(count);
    for (int i = 0; i < count; ++i)
    {
      original[i] = count - i;
    }
    for (const auto& algorithm : sortingAlgorithms)
    {
      std::array<double, 5> times{};
      for (double& time : times)
      {
        auto values = original; // Копирование не входит в измеряемый интервал.
        const auto start = std::chrono::steady_clock::now();
        algorithm.sort(values);
        const auto finish = std::chrono::steady_clock::now();
        time = std::chrono::duration<double, std::micro>(finish - start).count();
        // Для исходных N..1 известен весь ожидаемый результат, не только порядок.
        for (int i = 0; i < count; ++i)
        {
          if (values[i] != i + 1)
          {
            throw std::logic_error("Сортировка дала неверный результат");
          }
        }
      }
      // std::sort применяется только к пяти временам, сами данные сортируют наши функции.
      std::sort(times.begin(), times.end());
      out << std::fixed << std::setprecision(2) << std::setw(8) << count << std::setw(14) << algorithm.name
          << std::setw(16) << times[times.size() / 2] << '\n';
    }
  }
  out << "Подготовка массива, проверка результата и вывод находятся вне замера.\n";
}
