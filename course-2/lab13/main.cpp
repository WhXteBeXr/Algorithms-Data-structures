#include "Comparison.h"
#include "Graph.h"

#include <array>
#include <exception>
#include <iostream>
#include <limits>
#include <string_view>

namespace
{
  void printVertex(int vertex, std::string_view names)
  {
    if (names.empty())
    {
      std::cout << vertex;
    }
    else
    {
      std::cout << names[vertex];
    }
  }

  void printTree(std::string_view title, const MstResult& tree, std::string_view names = {})
  {
    std::cout << title << ", вес " << tree.weight << ", рёбер " << tree.edges.size() << '\n';
    for (const auto& edge : tree.edges)
    {
      printVertex(edge.from, names);
      std::cout << " -- ";
      printVertex(edge.to, names);
      std::cout << ": " << edge.weight << '\n';
    }
  }

  void printComponents(const std::vector<std::vector<int>>& components, std::string_view names = {})
  {
    std::cout << "Компонент связности: " << components.size() << '\n';
    for (std::size_t i = 0; i < components.size(); ++i)
    {
      std::cout << "Компонента " << i + 1 << ": ";
      for (int vertex : components[i])
      {
        printVertex(vertex, names);
        std::cout << ' ';
      }
      std::cout << '\n';
    }
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

  void demonstrateAlgorithms()
  {
    std::cout << "Часть 1. Прим O(V^2), вариант 3\n";
    Graph graph(5);
    const std::array<Edge, 7> edges{{
      {0, 1, 2}, {0, 3, 6}, {1, 2, 3}, {1, 3, 8}, {1, 4, 5}, {2, 4, 7}, {3, 4, 9}
    }};
    for (const auto& edge : edges)
    {
      graph.addEdge(edge.from, edge.to, edge.weight);
    }
    graph.print(std::cout);
    printTree("Дерево Прима", graph.prim());

    std::cout << "\nЧасть 2. Краскал и проверка связности, вариант 3\n";
    printComponents(graph.components());
    printTree("Дерево Краскала", graph.kruskal());
  }

  void planRoads(const Graph& graph, std::string_view cities)
  {
    const auto components = graph.components();
    if (components.size() != 1)
    {
      std::cout << "Соединить все города невозможно\n";
      printComponents(components, cities);
      return;
    }
    printTree("Дороги для строительства", graph.kruskal(), cities);
  }

  void demonstrateRoads()
  {
    std::cout << "\nЧасть 3. Прокладка дорог\nГорода: A B C D E F\n";
    Graph roads(6);
    const std::array<Edge, 7> edges{{
      {0, 1, 4}, {0, 2, 3}, {1, 2, 2}, {1, 3, 2}, {2, 4, 3}, {3, 4, 5}, {4, 5, 5}
    }};
    for (const auto& edge : edges)
    {
      roads.addEdge(edge.from, edge.to, edge.weight);
    }
    planRoads(roads, "ABCDEF"); // По указанным весам сумма равна 15.

    // Добавим пример с тремя компонентами, включая один изолированный город.
    Graph disconnected(7);
    disconnected.addEdge(0, 1, 4);
    disconnected.addEdge(1, 2, 2);
    disconnected.addEdge(3, 4, 1);
    disconnected.addEdge(4, 5, 3);
    std::cout << "Несвязная сеть городов A B C D E F G:\n";
    planRoads(disconnected, "ABCDEFG");
    showError("Прим несвязного графа", [&] { disconnected.prim(); });
    showError("Краскал несвязного графа", [&] { disconnected.kruskal(); });
  }

  void demonstrateBoundaries()
  {
    std::cout << "\nГраничные случаи\n";
    Graph special(3);
    special.addEdge(0, 1, 0);
    special.addEdge(1, 2, -4);
    special.addEdge(0, 2, 5);
    special.addEdge(1, 1, -100); // Петля не соединяет разные вершины и не входит в остов.
    special.addEdge(0, 1, 2); // Из параллельных рёбер полезнее вариант с нулевым весом.
    printTree("Прим: ноль, отрицательный вес, петля, параллельное ребро", special.prim());
    printTree("Краскал на том же графе", special.kruskal());

    Graph large(3);
    large.addEdge(0, 1, std::numeric_limits<int>::max());
    large.addEdge(1, 2, std::numeric_limits<int>::max());
    std::cout << "Два INT_MAX, веса MST: " << large.prim().weight << ", " << large.kruskal().weight << '\n';
    Graph single(1);
    std::cout << "Одна вершина, веса MST: " << single.prim().weight << ", " << single.kruskal().weight << '\n';
    Graph empty(0);
    printComponents(empty.components());
    showError("Прим пустого графа", [&] { empty.prim(); });
    showError("Краскал пустого графа", [&] { empty.kruskal(); });
    showError("Отрицательный размер", [] { Graph invalid(-1); });
    showError("Неверная вершина", [&] { special.addEdge(0, 3, 1); });
  }
}

int main()
{
  try
  {
    demonstrateAlgorithms();
    demonstrateRoads();
    demonstrateBoundaries();
    runComparison(std::cout);
  }
  catch (const std::exception& error)
  {
    std::cerr << "Ошибка: " << error.what() << '\n';
    return 1;
  }
}
