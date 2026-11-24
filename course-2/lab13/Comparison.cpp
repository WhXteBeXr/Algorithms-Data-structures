#include "Comparison.h"
#include "Graph.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <iomanip>
#include <ostream>
#include <random>
#include <stdexcept>

namespace
{
  // Создаёт связный граф из 100 вершин. Probability задаёт вероятность дополнительных рёбер в процентах.
  Graph randomGraph(int probability)
  {
    Graph graph(100);
    // Для каждой плотности фиксируем seed, чтобы повторный запуск получил тот же граф.
    std::mt19937 generator(2026 + probability);
    std::bernoulli_distribution hasEdge(probability / 100.0);
    std::uniform_int_distribution<int> weight(1, 100);
    for (int from = 0; from < graph.vertexCount(); ++from)
    {
      // Рассматриваем каждую неупорядоченную пару один раз, без петель и дублирования.
      for (int to = from + 1; to < graph.vertexCount(); ++to)
      {
        // Цепочка гарантирует связность. Остальные пары добавляются независимо.
        if (to == from + 1 || hasEdge(generator))
        {
          graph.addEdge(from, to, weight(generator));
        }
      }
    }
    return graph;
  }

  // Возвращает среднее время одного из 10 вызовов action в микросекундах.
  // Checksum по ссылке накапливает веса всех полученных остовов для последующей проверки.
  template <typename Action>
  double measure(Action action, std::int64_t& checksum)
  {
    const auto start = std::chrono::steady_clock::now();
    for (int repeat = 0; repeat < 10; ++repeat)
    {
      const auto result = action();
      checksum += result.weight; // Используется результат каждого вызова, не только последнего.
    }
    return std::chrono::duration<double, std::micro>(std::chrono::steady_clock::now() - start).count() / 10;
  }
}

// Сравнивает Прима и Краскала на одних графах трёх плотностей, печатает медианы пяти серий.
void runComparison(std::ostream& out)
{
  out << "\nСравнение производительности\n"
      << "V=100, связность обеспечена цепочкой. p - вероятность остальных рёбер.\n"
      << "Медиана 5 серий по 10 вызовов, время одного вызова в мкс.\n";
  out << std::fixed << std::setprecision(3);
  for (int probability : {20, 60, 90})
  {
    const auto graph = randomGraph(probability);
    // Сначала проверяем согласованность решений, только затем измеряем корректные алгоритмы.
    const auto prim = graph.prim();
    const auto kruskal = graph.kruskal();
    if (prim.weight != kruskal.weight || prim.edges.size() != 99 || kruskal.edges.size() != 99)
    {
      throw std::logic_error("Результаты алгоритмов MST не согласованы");
    }
    std::array<double, 5> primTimes;
    std::array<double, 5> kruskalTimes;
    std::int64_t primSum = 0;
    std::int64_t kruskalSum = 0;
    for (std::size_t trial = 0; trial < primTimes.size(); ++trial)
    {
      // Чередуем порядок, чтобы один алгоритм не измерялся постоянно первым.
      if (trial % 2 == 0)
      {
        primTimes[trial] = measure([&] { return graph.prim(); }, primSum);
        kruskalTimes[trial] = measure([&] { return graph.kruskal(); }, kruskalSum);
      }
      else
      {
        kruskalTimes[trial] = measure([&] { return graph.kruskal(); }, kruskalSum);
        primTimes[trial] = measure([&] { return graph.prim(); }, primSum);
      }
    }
    // Пять серий по десять вызовов должны дать 50 одинаковых весов у каждого алгоритма.
    if (primSum != 50 * prim.weight || kruskalSum != primSum)
    {
      throw std::logic_error("Не совпали контрольные суммы измерений");
    }
    // Центральное время упорядоченных серий меньше зависит от единичных задержек системы.
    std::sort(primTimes.begin(), primTimes.end());
    std::sort(kruskalTimes.begin(), kruskalTimes.end());
    out << "p=" << probability << "%, E=" << graph.edgeCount() << ", MST=" << prim.weight
        << ", Prim=" << primTimes[2] << ", Kruskal=" << kruskalTimes[2] << ", checksum=" << primSum << '\n';
  }
}
