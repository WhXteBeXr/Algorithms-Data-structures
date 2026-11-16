#include "Benchmark.h"
#include "DoublyLinkedList.h"
#include "SinglyLinkedList.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <iomanip>
#include <ostream>
#include <stdexcept>

namespace
{
  constexpr std::size_t repeatCount = 5;
  constexpr int accessCount = 256;
  constexpr int indexSpread = 32;

  struct Measurements
  {
    double pushBack;
    double popBack;
    double middle;
    double nearTail;
  };

  template <typename Action>
  double measure(Action action, std::int64_t expected)
  {
    const auto start = std::chrono::steady_clock::now();
    const std::int64_t result = action();
    const auto finish = std::chrono::steady_clock::now();
    // Проверка и печать находятся за границами измеряемого интервала.
    if (result != expected)
    {
      throw std::runtime_error("Неверный результат измеряемой серии");
    }
    return std::chrono::duration<double, std::micro>(finish - start).count();
  }

  template <typename List>
  Measurements runScenario(int count)
  {
    List values;
    Measurements result{};
    result.pushBack = measure([&]
    {
      for (int i = 0; i < count; ++i)
      {
        values.push_back(i);
      }
      return static_cast<std::int64_t>(values.size());
    }, count);

    // 256 обращений проходят восемь раз по 32 соседним индексам. Так нагрузка
    // не сводится к повторному чтению одного и того же элемента.
    constexpr std::int64_t offsetSum = (accessCount / indexSpread) * (indexSpread - 1) * indexSpread / 2;
    result.middle = measure([&]
    {
      std::int64_t sum = 0;
      for (int i = 0; i < accessCount; ++i)
      {
        sum += values[count / 2 + i % indexSpread];
      }
      return sum;
    }, static_cast<std::int64_t>(accessCount) * (count / 2) + offsetSum);

    result.nearTail = measure([&]
    {
      std::int64_t sum = 0;
      for (int i = 0; i < accessCount; ++i)
      {
        sum += values[count - 1 - i % indexSpread];
      }
      return sum;
    }, static_cast<std::int64_t>(accessCount) * (count - 1) - offsetSum);

    // Удаляем весь список с конца. Именно здесь n отдельных O(n) операций
    // односвязного списка дают O(n^2) для серии, у двусвязного получается O(n).
    result.popBack = measure([&]
    {
      std::int64_t removed = 0;
      while (!values.empty())
      {
        values.pop_back();
        ++removed;
      }
      return removed;
    }, count);
    return result;
  }

  double median(std::array<double, repeatCount> values)
  {
    std::sort(values.begin(), values.end());
    return values[repeatCount / 2];
  }

  Measurements collect(const std::array<Measurements, repeatCount>& samples)
  {
    std::array<double, repeatCount> push{}, pop{}, middle{}, tail{};
    for (std::size_t i = 0; i < repeatCount; ++i)
    {
      push[i] = samples[i].pushBack;
      pop[i] = samples[i].popBack;
      middle[i] = samples[i].middle;
      tail[i] = samples[i].nearTail;
    }
    return {median(push), median(pop), median(middle), median(tail)};
  }

  void printRow(std::ostream& out, int count, const char* kind, const Measurements& result)
  {
    out << std::fixed << std::setprecision(2) << std::setw(8) << count << std::setw(10) << kind
        << std::setw(14) << result.pushBack << std::setw(14) << result.popBack
        << std::setw(14) << result.middle << std::setw(14) << result.nearTail << '\n';
  }
}

void runListComparison(std::ostream& out)
{
  out << "\n=== Сравнение производительности ===\n"
      << "Микросекунды, медиана 5 запусков. Для сравнения используйте Release.\n"
      << "push/pop: серия из N операций. Индексы: 256 обращений в окне из 32 позиций.\n"
      << std::setw(8) << "N" << std::setw(10) << "List" << std::setw(14) << "push_back"
      << std::setw(14) << "pop_back" << std::setw(14) << "[] middle" << std::setw(14) << "[] near tail" << '\n';
  for (int count : {1000, 3000, 10000})
  {
    std::array<Measurements, repeatCount> singly{}, doubly{};
    for (std::size_t repeat = 0; repeat < repeatCount; ++repeat)
    {
      // Чередование уменьшает влияние постоянного первого/второго места запуска.
      if (repeat % 2 == 0)
      {
        singly[repeat] = runScenario<SinglyLinkedList<int>>(count);
        doubly[repeat] = runScenario<DoublyLinkedList<int>>(count);
      }
      else
      {
        doubly[repeat] = runScenario<DoublyLinkedList<int>>(count);
        singly[repeat] = runScenario<SinglyLinkedList<int>>(count);
      }
    }
    printRow(out, count, "Singly", collect(singly));
    printRow(out, count, "Doubly", collect(doubly));
  }
  out << "Середина: N/2 + i%32. Конец: N-1 - i%32, i от 0 до 255.\n"
      << "Проверяются размер, суммы прочитанных значений и число удалений.\n"
      << "Часы, системный аллокатор, нагрузка и кэш влияют на фактическое время.\n";
}
