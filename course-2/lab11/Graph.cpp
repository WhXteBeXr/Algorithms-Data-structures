#include "Graph.h"

#include <algorithm>
#include <ostream>
#include <queue>
#include <stdexcept>
#include <utility>

Graph::Graph(int vertices)
{
  if (vertices < 0)
  {
    throw std::invalid_argument("Количество вершин не может быть отрицательным");
  }
  adjacency.resize(static_cast<std::size_t>(vertices));
}

void Graph::checkVertex(int vertex) const
{
  if (vertex < 0 || vertex >= vertexCount())
  {
    throw std::out_of_range("Номер вершины должен быть в диапазоне 0..V-1");
  }
}

std::size_t Graph::edgeCount() const
{
  std::size_t count = 0;
  for (const auto& neighbors : adjacency)
  {
    count += neighbors.size();
  }
  return count;
}

bool Graph::addEdge(int from, int to)
{
  checkVertex(from);
  checkVertex(to);
  auto& neighbors = adjacency[from];
  if (std::find(neighbors.begin(), neighbors.end(), to) != neighbors.end())
  {
    return false;
  }
  // Сохраняем только from -> to. Обратное ребро само не появляется.
  neighbors.push_back(to);
  return true;
}

bool Graph::removeEdge(int from, int to)
{
  checkVertex(from);
  checkVertex(to);
  auto& neighbors = adjacency[from];
  const auto position = std::find(neighbors.begin(), neighbors.end(), to);
  if (position == neighbors.end())
  {
    return false;
  }
  neighbors.erase(position); // Порядок остальных соседей сохраняется.
  return true;
}

void Graph::print(std::ostream& out) const
{
  out << "V=" << vertexCount() << ", E=" << edgeCount() << '\n';
  for (int vertex = 0; vertex < vertexCount(); ++vertex)
  {
    out << vertex << ':';
    for (int neighbor : adjacency[vertex])
    {
      out << ' ' << neighbor;
    }
    out << '\n';
  }
}

void Graph::visit(const Adjacency& graph, int vertex, std::vector<bool>& visited, std::vector<int>& order)
{
  // Отметка до спуска предотвращает повторный вход через цикл или петлю.
  visited[vertex] = true;
  order.push_back(vertex);
  for (int neighbor : graph[vertex])
  {
    if (!visited[neighbor])
    {
      visit(graph, neighbor, visited, order);
    }
  }
}

std::vector<int> Graph::dfs(int start) const
{
  checkVertex(start);
  std::vector<bool> visited(vertexCount(), false);
  std::vector<int> order;
  visit(adjacency, start, visited, order);
  return order;
}

std::vector<int> Graph::dfsIterative(int start) const
{
  checkVertex(start);
  std::vector<bool> visited(vertexCount(), false);
  std::vector<int> order{start};
  // Кадр хранит вершину и индекс следующего соседа, как при остановке рекурсивного вызова.
  std::vector<std::pair<int, std::size_t>> stack{{start, 0}};
  visited[start] = true;
  while (!stack.empty())
  {
    auto& [vertex, next] = stack.back();
    if (next == adjacency[vertex].size())
    {
      stack.pop_back();
      continue;
    }
    const int neighbor = adjacency[vertex][next++];
    if (!visited[neighbor])
    {
      visited[neighbor] = true;
      order.push_back(neighbor);
      stack.emplace_back(neighbor, 0);
      // После emplace_back ссылки на старый кадр больше не используем: vector мог переразместиться.
    }
  }
  return order;
}

Graph::BfsResult Graph::runBfs(int start) const
{
  checkVertex(start);
  BfsResult result{{}, std::vector<int>(vertexCount(), -1)};
  std::queue<int> queue;
  // previous одновременно хранит предшественника и признак обнаружения вершины.
  result.previous[start] = start;
  queue.push(start);
  while (!queue.empty())
  {
    const int vertex = queue.front();
    queue.pop();
    result.order.push_back(vertex);
    for (int neighbor : adjacency[vertex])
    {
      if (result.previous[neighbor] == -1)
      {
        result.previous[neighbor] = vertex; // Помечаем при добавлении, а не при извлечении.
        queue.push(neighbor);
      }
    }
  }
  return result;
}

std::vector<int> Graph::bfs(int start) const
{
  return runBfs(start).order;
}

std::vector<std::vector<int>> Graph::weakComponents() const
{
  // Для слабой связности строим временную неориентированную проекцию.
  // Исходные направления остаются неизменными для обходов и кратчайшего пути.
  Adjacency weak = adjacency;
  for (int vertex = 0; vertex < vertexCount(); ++vertex)
  {
    for (int neighbor : adjacency[vertex])
    {
      if (vertex != neighbor)
      {
        weak[neighbor].push_back(vertex);
      }
    }
  }
  std::vector<bool> visited(vertexCount(), false);
  std::vector<std::vector<int>> components;
  for (int vertex = 0; vertex < vertexCount(); ++vertex)
  {
    if (!visited[vertex])
    {
      components.emplace_back();
      visit(weak, vertex, visited, components.back());
    }
  }
  return components;
}

std::vector<int> Graph::shortestPath(int start, int end) const
{
  checkVertex(end);
  const auto result = runBfs(start);
  if (result.previous[end] == -1)
  {
    return {};
  }
  // Восстанавливаем цепочку предшественников от цели до начала, затем разворачиваем.
  std::vector<int> path;
  for (int vertex = end; vertex != start; vertex = result.previous[vertex])
  {
    path.push_back(vertex);
  }
  path.push_back(start);
  std::reverse(path.begin(), path.end());
  return path;
}
