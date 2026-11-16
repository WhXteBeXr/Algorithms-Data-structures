#include "Benchmark.h"
#include "Stack.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <deque>
#include <iomanip>
#include <ostream>
#include <stack>
#include <stdexcept>
#include <vector>

namespace
{
  constexpr std::size_t repeatCount = 5;

  template <typename Action>
  double measureMicroseconds(Action action, std::int64_t expectedSum)
  {
    // steady_clock монотонны, перевод системных часов не искажает интервал.
    const auto start = std::chrono::steady_clock::now();
    const std::int64_t sum = action();
    const auto finish = std::chrono::steady_clock::now();

    // Результат вычисления используется. Проверка выполняется за пределами замера.
    if (sum != expectedSum)
    {
      throw std::runtime_error("Контрольная сумма извлечённых элементов неверна");
    }
    return std::chrono::duration<double, std::micro>(finish - start).count();
  }

  std::int64_t runCustomStack(int count)
  {
    Stack<int> stack;
    for (int i = 0; i < count; ++i)
    {
      stack.push(i);
    }
    std::int64_t sum = 0;
    while (!stack.isEmpty())
    {
      sum += stack.pop();
    }
    return sum;
  }

  template <typename Container>
  std::int64_t runStandardStack(int count)
  {
    std::stack<int, Container> stack;
    for (int i = 0; i < count; ++i)
    {
      stack.push(i);
    }
    std::int64_t sum = 0;
    while (!stack.empty())
    {
      // В STL pop() не возвращает значение, поэтому сначала вызывается top().
      sum += stack.top();
      stack.pop();
    }
    return sum;
  }

  double median(std::array<double, repeatCount> values)
  {
    std::sort(values.begin(), values.end());
    return values[repeatCount / 2];
  }
}

void runStackBenchmark(std::ostream& out)
{
  out << "\n=== Сравнение: N добавлений и N извлечений ===\n"
      << "Время в мкс, медиана из " << repeatCount << " запусков. Тип элементов: int.\n"
      << "Замер включает создание, расширения и уничтожение стека.\n";
  out << std::setw(10) << "N" << std::setw(14) << "Stack<T>" << std::setw(16) << "stack(deque)"
      << std::setw(16) << "stack(vector)" << std::setw(14) << "ns/op*" << std::setw(18) << "checksum" << '\n';

  for (int count : {10000, 100000, 1000000})
  {
    const std::int64_t expectedSum = static_cast<std::int64_t>(count) * (count - 1) / 2;
    std::array<std::array<double, repeatCount>, 3> times{};
    for (std::size_t repeat = 0; repeat < repeatCount; ++repeat)
    {
      // Меняем порядок запусков, чтобы один контейнер не измерялся всегда первым.
      for (std::size_t offset = 0; offset < times.size(); ++offset)
      {
        const std::size_t implementation = (repeat + offset) % times.size();
        if (implementation == 0)
        {
          times[0][repeat] = measureMicroseconds([count] { return runCustomStack(count); }, expectedSum);
        }
        else if (implementation == 1)
        {
          times[1][repeat] = measureMicroseconds(
              [count] { return runStandardStack<std::deque<int>>(count); }, expectedSum);
        }
        else
        {
          times[2][repeat] = measureMicroseconds(
              [count] { return runStandardStack<std::vector<int>>(count); }, expectedSum);
        }
      }
    }

    const double customTime = median(times[0]);
    // Условное среднее на операцию включает накладные расходы всей серии.
    const double nanosecondsPerOperation = customTime * 1000.0 / (2.0 * count);
    out << std::fixed << std::setprecision(2) << std::setw(10) << count << std::setw(14) << customTime
        << std::setw(16) << median(times[1]) << std::setw(16) << median(times[2])
        << std::setw(14) << nanosecondsPerOperation << std::setw(18) << expectedSum << '\n';
  }
  out << "* Среднее время на операцию своего стека в составе серии, а не отдельный замер push или pop.\n"
      << "Время зависит от компьютера, оптимизации и нагрузки. Для сравнения используйте Release.\n";
}
