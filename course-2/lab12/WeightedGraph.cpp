#include "WeightedGraph.h"

#include <algorithm>
#include <ostream>
#include <stdexcept>

WeightedGraph::WeightedGraph(int vertices, bool directed) : directed(directed)
{
  if (vertices < 0)
  {
    throw std::invalid_argument("Количество вершин не может быть отрицательным");
  }
  weights.assign(vertices, std::vector<Distance>(vertices, infinity));
}

void WeightedGraph::checkVertex(int vertex) const
{
  if (vertex < 0 || vertex >= vertexCount())
  {
    throw std::out_of_range("Номер вершины должен быть в диапазоне 0..V-1");
  }
}

void WeightedGraph::addEdge(int from, int to, int weight)
{
  checkVertex(from);
  checkVertex(to);
  weights[from][to] = weight;
  if (!directed)
  {
    weights[to][from] = weight;
  }
}

void WeightedGraph::print(std::ostream& out) const
{
  out << "V=" << vertexCount() << ", " << (directed ? "ориентированный" : "неориентированный") << '\n';
  for (int from = 0; from < vertexCount(); ++from)
  {
    for (int to = directed ? 0 : from; to < vertexCount(); ++to)
    {
      if (weights[from][to] != infinity)
      {
        out << from << (directed ? " -> " : " -- ") << to << ": " << weights[from][to] << '\n';
      }
    }
  }
}

WeightedGraph::Distance WeightedGraph::addDistances(Distance left, Distance right)
{
  // Конечные значения хранятся строго между -INF и INF. Их сумма помещается в int64_t.
  const Distance sum = left + right;
  if (sum <= -infinity || sum >= infinity)
  {
    throw std::overflow_error("Расстояние вышло за поддерживаемый диапазон");
  }
  return sum;
}

std::vector<WeightedGraph::Distance> WeightedGraph::dijkstra(int start) const
{
  checkVertex(start);
  // Проверяем весь граф, включая недостижимые из start дуги.
  for (const auto& row : weights)
  {
    if (std::any_of(row.begin(), row.end(), [](Distance weight) { return weight < 0; }))
    {
      throw std::domain_error("Дейкстра требует неотрицательные веса");
    }
  }
  std::vector<Distance> distance(vertexCount(), infinity);
  std::vector<bool> visited(vertexCount(), false);
  distance[start] = 0;
  for (int step = 0; step < vertexCount(); ++step)
  {
    int current = -1;
    for (int vertex = 0; vertex < vertexCount(); ++vertex)
    {
      if (!visited[vertex] && (current == -1 || distance[vertex] < distance[current]))
      {
        current = vertex;
      }
    }
    if (current == -1 || distance[current] == infinity)
    {
      break; // Оставшиеся вершины недостижимы, INF не участвует в сложении.
    }
    visited[current] = true;
    for (int to = 0; to < vertexCount(); ++to)
    {
      if (!visited[to] && weights[current][to] != infinity)
      {
        const Distance candidate = addDistances(distance[current], weights[current][to]);
        if (candidate < distance[to])
        {
          distance[to] = candidate;
        }
      }
    }
  }
  return distance;
}

WeightedGraph::FloydResult WeightedGraph::floydWarshall() const
{
  const int vertices = vertexCount();
  FloydResult result{weights, std::vector<std::vector<int>>(vertices, std::vector<int>(vertices, -1))};
  for (int from = 0; from < vertices; ++from)
  {
    // Нулевой путь существует без петли. Отрицательную петлю нельзя заменить нулём.
    result.distance[from][from] = std::min(Distance{0}, result.distance[from][from]);
    for (int to = 0; to < vertices; ++to)
    {
      if (result.distance[from][to] != infinity)
      {
        result.next[from][to] = to;
      }
    }
  }
  for (int via = 0; via < vertices; ++via)
  {
    // Внешний цикл расширяет множество допустимых промежуточных вершин.
    for (int from = 0; from < vertices; ++from)
    {
      if (result.distance[from][via] == infinity)
      {
        continue;
      }
      for (int to = 0; to < vertices; ++to)
      {
        if (result.distance[via][to] != infinity)
        {
          const Distance candidate = addDistances(result.distance[from][via], result.distance[via][to]);
          if (candidate < result.distance[from][to])
          {
            result.distance[from][to] = candidate;
            result.next[from][to] = result.next[from][via]; // Первый шаг берётся из первой части пути.
          }
        }
      }
    }
    // При отрицательном цикле конечного минимума нет. Не выдаём некорректную цепочку next.
    for (int vertex = 0; vertex < vertices; ++vertex)
    {
      if (result.distance[vertex][vertex] < 0)
      {
        throw std::domain_error("Обнаружен отрицательный цикл");
      }
    }
  }
  return result;
}

std::vector<int> WeightedGraph::FloydResult::path(int from, int to) const
{
  const int vertices = static_cast<int>(distance.size());
  if (from < 0 || to < 0 || from >= vertices || to >= vertices)
  {
    throw std::out_of_range("Неверная вершина при восстановлении пути");
  }
  if (distance[from][to] == infinity)
  {
    return {};
  }
  std::vector<int> route{from};
  while (from != to)
  {
    from = next[from][to];
    route.push_back(from);
  }
  return route;
}
