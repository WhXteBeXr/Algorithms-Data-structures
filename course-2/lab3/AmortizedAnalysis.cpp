#include "AmortizedAnalysis.h"
#include "Vector.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <iomanip>
#include <ostream>
#include <stdexcept>

void runAmortizedAnalysis(std::ostream& out)
{
  out << "\n=== Амортизированный анализ push_back ===\n";
  Vector<int> example;
  for (int value = 1; value <= 8; ++value)
  {
    const std::size_t before = example.getCopyCount();
    example.push_back(value);
    out << "Вставка " << value << ": копирований = " << example.getCopyCount() - before
        << ", ёмкость = " << example.capacity() << '\n';
  }

  out << "\nВремя добавлений в мкс, медиана 5 запусков. Проверка суммы вне замера.\n"
      << std::setw(10) << "N" << std::setw(12) << "copies" << std::setw(12) << "N+copies"
      << std::setw(12) << "ops/push" << std::setw(14) << "time_us" << std::setw(14) << "ns/push" << '\n';

  for (int count : {1000, 10000, 100000, 1000000})
  {
    std::array<double, 5> times{};
    std::size_t copies = 0;
    for (double& time : times)
    {
      Vector<int> values;
      const auto start = std::chrono::steady_clock::now();
      for (int i = 0; i < count; ++i)
      {
        values.push_back(i);
      }
      const auto finish = std::chrono::steady_clock::now();
      time = std::chrono::duration<double, std::micro>(finish - start).count();
      copies = values.getCopyCount();

      // Сумма использует фактическое содержимое массива. Её расчёт и удаление
      // контейнера не входят в замер, включающий только серию push_back.
      const std::int64_t expected = static_cast<std::int64_t>(count) * (count - 1) / 2;
      if (values.sum() != static_cast<long double>(expected))
      {
        throw std::runtime_error("Неверная сумма элементов после добавления");
      }
    }

    std::sort(times.begin(), times.end());
    const double medianTime = times[times.size() / 2];
    const std::size_t operations = static_cast<std::size_t>(count) + copies;
    out << std::fixed << std::setprecision(2) << std::setw(10) << count << std::setw(12) << copies
        << std::setw(12) << operations << std::setw(12) << static_cast<double>(operations) / count
        << std::setw(14) << medianTime << std::setw(14) << medianTime * 1000.0 / count << '\n';
  }
  out << "copies учитывает перенос прежних элементов, без добавляемого элемента.\n"
      << "N+copies - модель затрат, а не число всех машинных инструкций.\n"
      << "Замеры зависят от компьютера и нагрузки.\n";
}
