#include "Graph.h"

#include <array>
#include <exception>
#include <iostream>
#include <span>
#include <string_view>
#include <utility>

namespace
{
  void printVertices(std::string_view label, std::span<const int> vertices)
  {
    std::cout << label;
    if (vertices.empty())
    {
      std::cout << "(пусто)";
    }
    for (int vertex : vertices)
    {
      std::cout << vertex << ' ';
    }
    std::cout << '\n';
  }

  void printComponents(const Graph& graph)
  {
    const auto components = graph.weakComponents();
    std::cout << "Компонент слабой связности: " << components.size() << '\n';
    for (std::size_t i = 0; i < components.size(); ++i)
    {
      std::cout << "Компонента " << i + 1 << ": ";
      printVertices("", components[i]);
    }
  }

  void printPath(const Graph& graph, int start, int end)
  {
    const auto path = graph.shortestPath(start, end);
    std::cout << "Путь " << start << " -> " << end << ": ";
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
    std::cout << ", длина " << path.size() - 1 << '\n';
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

  void demonstrateGraph()
  {
    std::cout << "Часть 1. Ориентированный граф, вариант 3\n";
    Graph graph(9);
    const std::array<std::pair<int, int>, 9> edges{{
      {0, 1}, {0, 2}, {1, 3}, {2, 3}, {3, 1}, {3, 4}, {4, 5}, {5, 2}, {7, 6}
    }};
    for (const auto& [from, to] : edges)
    {
      graph.addEdge(from, to);
    }
    // Есть циклы, отдельная дуга 7 -> 6 и изолированная вершина 8.
    graph.print(std::cout);
    std::cout << "Повторное ребро 0 -> 1: " << graph.addEdge(0, 1)
              << ", E=" << graph.edgeCount() << '\n';

    std::cout << "\nЧасть 2. DFS, BFS и компоненты, вариант 3\n";
    printVertices("DFS из 0: ", graph.dfs(0));
    printVertices("DFS со стеком из 0: ", graph.dfsIterative(0));
    printVertices("BFS из 0: ", graph.bfs(0));
    printVertices("DFS из 6: ", graph.dfs(6));
    printVertices("BFS из 7: ", graph.bfs(7));
    printComponents(graph); // Слабая связность объединяет 6 и 7 независимо от направления.

    std::cout << "\nЧасть 3. Кратчайшие направленные пути\n";
    printPath(graph, 0, 5); // Есть два одинаково коротких пути, выбирается первый по порядку соседей.
    printPath(graph, 5, 0); // Обратная достижимость не следует из прямой.
    printPath(graph, 7, 6);
    printPath(graph, 6, 7);
    printPath(graph, 0, 8);
    printPath(graph, 8, 8); // Нулевой путь существует и у изолированной вершины.

    std::cout << "\nГраничные случаи и изменение графа\n";
    // Удаляем только одну дугу. Обратная 3 -> 1 остаётся и сохраняет слабую связность.
    std::cout << "Удаление 1 -> 3: " << graph.removeEdge(1, 3)
              << ", повторное: " << graph.removeEdge(1, 3) << '\n';
    graph.print(std::cout);
    printPath(graph, 0, 5);
    printComponents(graph);

    Graph single(1);
    single.addEdge(0, 0); // Петля не должна приводить к повторным посещениям.
    printVertices("DFS одного узла с петлёй: ", single.dfs(0));
    printVertices("DFS со стеком с петлёй: ", single.dfsIterative(0));
    printVertices("BFS одного узла с петлёй: ", single.bfs(0));
    printPath(single, 0, 0);
    printComponents(single);

    Graph empty(0);
    std::cout << "Пустой граф: ";
    empty.print(std::cout);
    printComponents(empty);
    showError("DFS пустого графа", [&] { empty.dfs(0); });
    showError("BFS пустого графа", [&] { empty.bfs(0); });
    showError("Отрицательный размер", [] { Graph invalid(-1); });
    showError("Неверное начало DFS", [&] { graph.dfs(-1); });
    showError("Неверный конец пути", [&] { graph.shortestPath(0, 9); });
    showError("Неверное ребро", [&] { graph.addEdge(0, 9); });
    std::cout << "После ошибочного добавления E=" << graph.edgeCount() << '\n';
  }
}

int main()
{
  std::cout << std::boolalpha;
  try
  {
    demonstrateGraph();
  }
  catch (const std::exception& error)
  {
    std::cerr << "Ошибка: " << error.what() << '\n';
    return 1;
  }
}
