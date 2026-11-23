#include "Graph.h"

#include <algorithm>
#include <limits>
#include <numeric>
#include <ostream>
#include <stdexcept>
#include <tuple>
#include <utility>

Graph::Graph(int vertices) : vertices(vertices)
{
  if (vertices < 0)
  {
    throw std::invalid_argument("Количество вершин не может быть отрицательным");
  }
}

void Graph::checkVertex(int vertex) const
{
  if (vertex < 0 || vertex >= vertices)
  {
    throw std::out_of_range("Номер вершины должен быть в диапазоне 0..V-1");
  }
}

void Graph::addEdge(int from, int to, int weight)
{
  checkVertex(from);
  checkVertex(to);
  // У неориентированного ребра нет направления. Нормализуем только запись концов.
  if (from > to)
  {
    std::swap(from, to);
  }
  connections.push_back({from, to, weight});
}

void Graph::print(std::ostream& out) const
{
  out << "V=" << vertices << ", E=" << connections.size() << '\n';
  for (const auto& edge : connections)
  {
    out << edge.from << " -- " << edge.to << ": " << edge.weight << '\n';
  }
}

MstResult Graph::prim(int start) const
{
  checkVertex(start);
  using Weight = std::int64_t;
  constexpr Weight infinity = std::numeric_limits<Weight>::max();
  std::vector<std::vector<Weight>> matrix(vertices, std::vector<Weight>(vertices, infinity));
  for (const auto& edge : connections)
  {
    // Матрица для Прима хранит минимум из параллельных рёбер, включая нулевые веса.
    const Weight weight = std::min(matrix[edge.from][edge.to], Weight{edge.weight});
    matrix[edge.from][edge.to] = matrix[edge.to][edge.from] = weight;
  }
  std::vector<Weight> minEdge(vertices, infinity);
  std::vector<int> parent(vertices, -1);
  std::vector<bool> included(vertices, false);
  minEdge[start] = 0;
  MstResult result;
  for (int step = 0; step < vertices; ++step)
  {
    int current = -1;
    for (int vertex = 0; vertex < vertices; ++vertex)
    {
      if (!included[vertex] && (current == -1 || minEdge[vertex] < minEdge[current]))
      {
        current = vertex;
      }
    }
    if (current == -1 || minEdge[current] == infinity)
    {
      throw std::domain_error("Граф не связный: MST не существует");
    }
    included[current] = true;
    if (parent[current] != -1)
    {
      // parent запоминает выбранную дорогу, корень не добавляет искусственное ребро.
      result.edges.push_back({parent[current], current, static_cast<int>(minEdge[current])});
      result.weight += minEdge[current];
    }
    for (int to = 0; to < vertices; ++to)
    {
      // Храним вес одного ребра из дерева, а не расстояние по пути, как у Дейкстры.
      if (!included[to] && matrix[current][to] < minEdge[to])
      {
        minEdge[to] = matrix[current][to];
        parent[to] = current;
      }
    }
  }
  return result;
}

bool Graph::merge(std::vector<int>& labels, int from, int to)
{
  int keep = labels[from];
  int replace = labels[to];
  if (keep == replace)
  {
    return false;
  }
  if (keep > replace)
  {
    std::swap(keep, replace);
  }
  // Все вершины объединённой компоненты сразу получают одну общую метку.
  // Это простая реализация без деревьев родителей, рангов и сжатия путей DSU.
  for (int& label : labels)
  {
    if (label == replace)
    {
      label = keep;
    }
  }
  return true;
}

MstResult Graph::kruskal() const
{
  if (vertices == 0)
  {
    throw std::domain_error("Для пустого графа MST не определено");
  }
  auto sorted = connections; // Сортируем копию, исходный порядок дорог сохраняется.
  std::sort(sorted.begin(), sorted.end(), [](const Edge& left, const Edge& right)
  {
    return std::tie(left.weight, left.from, left.to) < std::tie(right.weight, right.from, right.to);
  });
  std::vector<int> labels(vertices);
  std::iota(labels.begin(), labels.end(), 0);
  MstResult result;
  for (const auto& edge : sorted)
  {
    if (merge(labels, edge.from, edge.to))
    {
      result.edges.push_back(edge);
      result.weight += edge.weight;
      if (result.edges.size() == static_cast<std::size_t>(vertices - 1))
      {
        break;
      }
    }
  }
  if (result.edges.size() != static_cast<std::size_t>(vertices - 1))
  {
    throw std::domain_error("Граф не связный: MST не существует");
  }
  return result;
}

std::vector<std::vector<int>> Graph::components() const
{
  std::vector<int> labels(vertices);
  std::iota(labels.begin(), labels.end(), 0);
  for (const auto& edge : connections)
  {
    merge(labels, edge.from, edge.to);
  }
  std::vector<std::vector<int>> groups(vertices);
  for (int vertex = 0; vertex < vertices; ++vertex)
  {
    groups[labels[vertex]].push_back(vertex);
  }
  // Метки равны минимальным номерам компонент, пустые промежутки удаляются.
  std::erase_if(groups, [](const auto& group) { return group.empty(); });
  return groups;
}
