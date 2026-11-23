#pragma once

#include <cstddef>
#include <cstdint>
#include <iosfwd>
#include <vector>

struct Edge
{
  int from;
  int to;
  int weight;
};

struct MstResult
{
  std::vector<Edge> edges;
  std::int64_t weight = 0;
};

// Неориентированный взвешенный граф, вершины 0..V-1.
class Graph
{
public:
  explicit Graph(int vertices);
  int vertexCount() const { return vertices; }
  std::size_t edgeCount() const { return connections.size(); }
  // Параллельные рёбра и петли допустимы, алгоритмы выберут полезные связи.
  void addEdge(int from, int to, int weight);
  void print(std::ostream& out) const;
  MstResult prim(int start = 0) const;
  MstResult kruskal() const;
  std::vector<std::vector<int>> components() const;

private:
  int vertices;
  std::vector<Edge> connections;
  void checkVertex(int vertex) const;
  // Простые метки компонент: проверка O(1), успешное объединение O(V).
  static bool merge(std::vector<int>& labels, int from, int to);
};
