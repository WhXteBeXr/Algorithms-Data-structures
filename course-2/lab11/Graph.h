#pragma once

#include <cstddef>
#include <iosfwd>
#include <vector>

// Ориентированный невзвешенный граф. Номера вершин фиксированы: 0..V-1.
class Graph
{
public:
  explicit Graph(int vertices);
  int vertexCount() const { return static_cast<int>(adjacency.size()); }
  std::size_t edgeCount() const;
  bool addEdge(int from, int to);
  bool removeEdge(int from, int to);
  void print(std::ostream& out) const;

  std::vector<int> dfs(int start) const;
  std::vector<int> dfsIterative(int start) const;
  std::vector<int> bfs(int start) const;
  std::vector<std::vector<int>> weakComponents() const;
  // Пустой результат означает отсутствие направленного пути, start == end даёт {start}.
  std::vector<int> shortestPath(int start, int end) const;

private:
  using Adjacency = std::vector<std::vector<int>>;
  Adjacency adjacency;
  struct BfsResult
  {
    std::vector<int> order;
    std::vector<int> previous;
  };

  void checkVertex(int vertex) const;
  static void visit(const Adjacency& graph, int vertex, std::vector<bool>& visited, std::vector<int>& order);
  BfsResult runBfs(int start) const;
};
