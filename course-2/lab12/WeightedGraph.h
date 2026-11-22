#pragma once

#include <cstdint>
#include <iosfwd>
#include <limits>
#include <vector>

class WeightedGraph
{
public:
  using Distance = std::int64_t;
  using Matrix = std::vector<std::vector<Distance>>;
  // Общая неизменяемая метка отсутствия пути. Ноль остаётся допустимым весом.
  static constexpr Distance infinity = std::numeric_limits<Distance>::max() / 4;

  struct FloydResult
  {
    Matrix distance;
    std::vector<std::vector<int>> next;
    std::vector<int> path(int from, int to) const;
  };

  explicit WeightedGraph(int vertices, bool directed = true);
  int vertexCount() const { return static_cast<int>(weights.size()); }
  // Повторное добавление заменяет вес, в неориентированном режиме в обе стороны.
  void addEdge(int from, int to, int weight);
  void print(std::ostream& out) const;
  std::vector<Distance> dijkstra(int start) const;
  FloydResult floydWarshall() const;

private:
  Matrix weights;
  bool directed;
  void checkVertex(int vertex) const;
  static Distance addDistances(Distance left, Distance right);
};
