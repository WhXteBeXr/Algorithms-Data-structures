#include "Comparison.h"
#include "WeightedGraph.h"

#include <exception>
#include <iomanip>
#include <iostream>
#include <limits>
#include <span>
#include <string_view>

namespace
{
  void printDistance(WeightedGraph::Distance value)
  {
    if (value == WeightedGraph::infinity)
    {
      std::cout << "INF";
    }
    else
    {
      std::cout << value;
    }
  }

  void printDistances(std::span<const WeightedGraph::Distance> distances)
  {
    for (std::size_t to = 0; to < distances.size(); ++to)
    {
      std::cout << "До " << to << ": ";
      printDistance(distances[to]);
      std::cout << '\n';
    }
  }

  void printPath(const WeightedGraph::FloydResult& result, int from, int to)
  {
    const auto path = result.path(from, to);
    std::cout << "Путь " << from << " -> " << to << ": ";
    if (path.empty())
    {
      std::cout << "не существует\n";
      return;
    }
    for (std::size_t i = 0; i < path.size(); ++i)
    {
      if (i != 0)
      {
        std::cout << " -> ";
      }
      std::cout << path[i];
    }
    std::cout << ", вес " << result.distance[from][to] << '\n';
  }

  template <typename Action>
  void showError(std::string_view label, Action action)
  {
    try
    {
      action();
    }
    catch (const std::exception& error)
    {
      std::cout << label << ": " << error.what() << '\n';
    }
  }

  void demonstrateDijkstra()
  {
    std::cout << "Часть 1. Дейкстра O(V^2), вариант 3\n";
    WeightedGraph graph(7);
    // Направление 2 -> 1 позволяет улучшить путь к 1 через 2. Вершина 6 изолирована.
    graph.addEdge(0, 1, 4);
    graph.addEdge(0, 2, 2);
    graph.addEdge(2, 1, 1);
    graph.addEdge(1, 3, 5);
    graph.addEdge(2, 3, 8);
    graph.addEdge(2, 4, 10);
    graph.addEdge(3, 4, 2);
    graph.addEdge(3, 5, 6);
    graph.addEdge(4, 5, 3);
    graph.print(std::cout);
    std::cout << "Расстояния из 0:\n";
    printDistances(graph.dijkstra(0));
    std::cout << "Из 5 до 0: ";
    printDistance(graph.dijkstra(5)[0]); // Обратного направления нет.
    std::cout << '\n';
  }

  void demonstrateFloyd()
  {
    std::cout << "\nЧасть 2. Флойд-Уоршелл и пути, вариант 3\n";
    WeightedGraph graph(6, false);
    // В неориентированном режиме каждая запись задаёт оба направления.
    graph.addEdge(0, 1, 2);
    graph.addEdge(0, 2, 5);
    graph.addEdge(1, 2, 1);
    graph.addEdge(1, 3, 6);
    graph.addEdge(2, 3, 2);
    graph.addEdge(2, 4, 3);
    graph.addEdge(3, 4, 1);
    const auto result = graph.floydWarshall();
    std::cout << "Неориентированный граф, вершина 5 изолирована\nМатрица расстояний:\n";
    for (const auto& row : result.distance)
    {
      for (auto value : row)
      {
        std::cout << std::setw(5);
        printDistance(value);
      }
      std::cout << '\n';
    }
    printPath(result, 0, 4);
    printPath(result, 4, 0);
    printPath(result, 0, 5);
    printPath(result, 5, 5);
  }

  void demonstrateBoundaries()
  {
    std::cout << "\nГраничные случаи и отрицательные веса\n";
    WeightedGraph zero(3);
    zero.addEdge(0, 1, 0); // Нулевой вес не является признаком отсутствия дуги.
    zero.addEdge(1, 2, 7);
    std::cout << "Нулевой вес, расстояния из 0:\n";
    printDistances(zero.dijkstra(0));
    printPath(zero.floydWarshall(), 0, 2);

    WeightedGraph large(3);
    large.addEdge(0, 1, std::numeric_limits<int>::max());
    large.addEdge(1, 2, std::numeric_limits<int>::max());
    std::cout << "Два веса INT_MAX, расстояние до 2: " << large.dijkstra(0)[2] << '\n';
    printPath(large.floydWarshall(), 0, 2);

    // Отрицательная дуга допустима для Флойда, если не образует отрицательного цикла.
    WeightedGraph negative(4);
    negative.addEdge(0, 1, 4);
    negative.addEdge(0, 2, 5);
    negative.addEdge(1, 2, -2);
    negative.addEdge(2, 3, 3);
    printPath(negative.floydWarshall(), 0, 3);
    showError("Дейкстра с отрицательной дугой", [&] { negative.dijkstra(0); });
    WeightedGraph cycle(2, false);
    cycle.addEdge(0, 1, -1); // Два направления дают замкнутый маршрут отрицательного веса.
    showError("Флойд с отрицательным циклом", [&] { cycle.floydWarshall(); });

    WeightedGraph single(1);
    std::cout << "Один узел, расстояние до себя: " << single.dijkstra(0)[0] << '\n';
    printPath(single.floydWarshall(), 0, 0);
    WeightedGraph empty(0);
    std::cout << "Пустой граф, строк результата Флойда: " << empty.floydWarshall().distance.size() << '\n';
    showError("Дейкстра пустого графа", [&] { empty.dijkstra(0); });
    showError("Путь пустого графа", [&] { empty.floydWarshall().path(0, 0); });
    showError("Отрицательный размер", [] { WeightedGraph invalid(-1); });
    showError("Неверная вершина", [&] { zero.addEdge(0, 3, 1); });
  }
}

int main()
{
  try
  {
    demonstrateDijkstra();
    demonstrateFloyd();
    demonstrateBoundaries();
    runComparison(std::cout);
  }
  catch (const std::exception& error)
  {
    std::cerr << "Ошибка: " << error.what() << '\n';
    return 1;
  }
}
