#include "Comparison.h"
#include "WeightedGraph.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <iomanip>
#include <ostream>
#include <random>
#include <span>
#include <stdexcept>

namespace
{
  WeightedGraph randomGraph(int vertices, std::mt19937& generator)
  {
    WeightedGraph graph(vertices);
    std::bernoulli_distribution hasEdge(0.25);
    std::uniform_int_distribution<int> weight(1, 20);
    for (int from = 0; from < vertices; ++from)
    {
      for (int to = 0; to < vertices; ++to)
      {
        if (from != to && hasEdge(generator))
        {
          graph.addEdge(from, to, weight(generator));
        }
      }
    }
    return graph;
  }

  std::uint64_t rowSum(std::span<const WeightedGraph::Distance> row)
  {
    std::uint64_t sum = 0;
    for (auto distance : row)
    {
      // Ноль кодирует недостижимость, конечное нулевое расстояние даёт единицу.
      sum += distance == WeightedGraph::infinity ? 0 : static_cast<std::uint64_t>(distance) + 1;
    }
    return sum;
  }

  template <typename Action>
  double measure(Action action, std::uint64_t& checksum)
  {
    const auto start = std::chrono::steady_clock::now();
    for (int repeat = 0; repeat < 10; ++repeat)
    {
      checksum += action();
    }
    return std::chrono::duration<double, std::micro>(std::chrono::steady_clock::now() - start).count() / 10;
  }

  struct Row
  {
    int vertices;
    int start;
    double dijkstra;
    double floyd;
    std::uint64_t checksum;
  };
}

void runComparison(std::ostream& out)
{
  out << "\nЧасть 3. Случайные данные и сравнение\n";
  std::array<Row, 4> rows;
  std::size_t index = 0;
  for (int vertices : {10, 20, 50, 100})
  {
    std::mt19937 generator(2026 + vertices);
    const auto graph = randomGraph(vertices, generator);
    const int start = std::uniform_int_distribution<int>(0, vertices - 1)(generator);
    const auto distance = graph.dijkstra(start);
    const auto all = graph.floydWarshall();
    // Сравнивается вся строка тех же пар (start, v), а не только итоговая сумма.
    if (distance != all.distance[start])
    {
      throw std::logic_error("Расстояния Дейкстры и Флойда не совпадают");
    }
    if (vertices == 20)
    {
      out << "Граф V=20: ориентированный, вероятность дуги 0.25, веса 1..20\n"
          << "Seed=2046, случайный start=" << start << '\n';
      for (int to = 0; to < vertices; ++to)
      {
        out << start << " -> " << to << ": Dijkstra=";
        if (distance[to] == WeightedGraph::infinity)
        {
          out << "INF, Floyd=INF\n";
        }
        else
        {
          out << distance[to] << ", Floyd=" << all.distance[start][to] << '\n';
        }
      }
      out << "Все 20 расстояний совпали\n";
    }

    std::array<double, 5> dijkstraTimes;
    std::array<double, 5> floydTimes;
    std::uint64_t dijkstraSum = 0;
    std::uint64_t floydSum = 0;
    const auto timeDijkstra = [&]
    {
      return measure([&] { return rowSum(graph.dijkstra(start)); }, dijkstraSum);
    };
    const auto timeFloyd = [&]
    {
      return measure([&] { return rowSum(graph.floydWarshall().distance[start]); }, floydSum);
    };
    for (std::size_t trial = 0; trial < dijkstraTimes.size(); ++trial)
    {
      // Чередование снижает зависимость результата от постоянного порядка запуска.
      if (trial % 2 == 0)
      {
        dijkstraTimes[trial] = timeDijkstra();
        floydTimes[trial] = timeFloyd();
      }
      else
      {
        floydTimes[trial] = timeFloyd();
        dijkstraTimes[trial] = timeDijkstra();
      }
    }
    if (dijkstraSum != 50 * rowSum(distance) || dijkstraSum != floydSum)
    {
      throw std::logic_error("Не совпали контрольные суммы замеров");
    }
    std::sort(dijkstraTimes.begin(), dijkstraTimes.end());
    std::sort(floydTimes.begin(), floydTimes.end());
    rows[index++] = {vertices, start, dijkstraTimes[2], floydTimes[2], dijkstraSum};
  }
  out << "Время одного вызова, мкс: медиана 5 серий по 10 вызовов\n"
      << "Дейкстра: один источник. Флойд: все пары.\n" << std::fixed << std::setprecision(3);
  for (const auto& row : rows)
  {
    out << "V=" << row.vertices << ", start=" << row.start << ", Dijkstra=" << row.dijkstra
        << ", Floyd=" << row.floyd << ", checksum=" << row.checksum << '\n';
  }
}
