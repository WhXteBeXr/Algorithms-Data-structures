#include "Comparison.h"
#include "BinarySearch.h"
#include "HashTable.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <iomanip>
#include <ostream>
#include <random>
#include <stdexcept>
#include <vector>

namespace
{
  template <typename Action>
  double measure(Action action, std::uint64_t expected)
  {
    const auto start = std::chrono::steady_clock::now();
    const std::uint64_t sum = action();
    const auto finish = std::chrono::steady_clock::now();
    if (sum != expected)
    {
      throw std::logic_error("Контрольная сумма поиска неверна");
    }
    return std::chrono::duration<double, std::micro>(finish - start).count();
  }

  double median(std::array<double, 5> times)
  {
    std::sort(times.begin(), times.end());
    return times[times.size() / 2];
  }
}

void compareSearch(std::ostream& out)
{
  out << "\n=== Часть 3. Сравнение: 10000 запросов, медиана 5 запусков, мкс ===\n"
      << std::setw(9) << "N" << std::setw(12) << "Capacity" << std::setw(14) << "Binary_us"
      << std::setw(14) << "Hash_us" << std::setw(16) << "Checksum" << '\n';
  std::mt19937 generator(2026);
  for (int count : {1000, 10000, 100000})
  {
    std::vector<int> sorted(count);
    HashTable table(count * 2 + 1);
    for (int i = 0; i < count; ++i)
    {
      sorted[i] = i * 2;
      table.insert(i * 2, i);
    }
    std::uniform_int_distribution<int> queryValue(0, count * 2 - 1);
    std::vector<int> queries(10000);
    std::uint64_t expected = 0;
    for (int& key : queries)
    {
      key = queryValue(generator);
      // Чётные ключи присутствуют, нечётные отсутствуют. +1 отличает индекс 0 от промаха.
      if (key % 2 == 0)
      {
        expected += static_cast<std::uint64_t>(key / 2) + 1;
      }
    }
    const auto binaryAction = [&]
    {
      std::uint64_t sum = 0;
      for (int key : queries)
      {
        const auto index = firstOccurrence(sorted, key);
        if (index >= 0)
        {
          sum += static_cast<std::uint64_t>(index) + 1;
        }
      }
      return sum;
    };
    const auto hashAction = [&]
    {
      std::uint64_t sum = 0;
      for (int key : queries)
      {
        if (const int* value = table.find(key))
        {
          sum += static_cast<std::uint64_t>(*value) + 1;
        }
      }
      return sum;
    };
    std::array<double, 5> binaryTimes{}, hashTimes{};
    for (std::size_t run = 0; run < binaryTimes.size(); ++run)
    {
      // Чередуем порядок, все повторы получают один и тот же набор запросов.
      if (run % 2 == 0)
      {
        binaryTimes[run] = measure(binaryAction, expected);
        hashTimes[run] = measure(hashAction, expected);
      }
      else
      {
        hashTimes[run] = measure(hashAction, expected);
        binaryTimes[run] = measure(binaryAction, expected);
      }
    }
    out << std::fixed << std::setprecision(2) << std::setw(9) << count << std::setw(12) << table.capacity()
        << std::setw(14) << median(binaryTimes) << std::setw(14) << median(hashTimes)
        << std::setw(16) << expected << '\n';
  }
  out << "Ключи 0, 2, ..., 2(N-1). Запросы смешивают попадания и промахи.\n"
      << "Подготовка и проверка суммы вне замера, поиск в фиксированной таблице без удалений.\n";
}
