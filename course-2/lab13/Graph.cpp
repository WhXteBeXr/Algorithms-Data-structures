#include "Graph.h"

#include <algorithm>
#include <limits>
#include <numeric>
#include <ostream>
#include <stdexcept>
#include <tuple>
#include <utility>

// Создаёт граф с вершинами 0..vertices-1, рёбер изначально нет. Отрицательный размер запрещён.
Graph::Graph(int vertices) : vertices(vertices)
{
  if (vertices < 0)
  {
    throw std::invalid_argument("Количество вершин не может быть отрицательным");
  }
}

// Проверяет номер вершины до обращения к рабочим массивам алгоритмов.
void Graph::checkVertex(int vertex) const
{
  if (vertex < 0 || vertex >= vertices)
  {
    throw std::out_of_range("Номер вершины должен быть в диапазоне 0..V-1");
  }
}

// Добавляет неориентированное ребро с заданным весом. Петли и параллельные рёбра допустимы.
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

// Выводит размеры графа и список всех хранимых рёбер, не меняя их порядок.
void Graph::print(std::ostream& out) const
{
  out << "V=" << vertices << ", E=" << connections.size() << '\n';
  for (const auto& edge : connections)
  {
    out << edge.from << " -- " << edge.to << ": " << edge.weight << '\n';
  }
}

// Строит минимальное остовное дерево от start и возвращает его рёбра и суммарный вес.
// Для несвязного графа бросает domain_error, исходный граф не изменяет.
MstResult Graph::prim(int start) const
{
  checkVertex(start);
  using Weight = std::int64_t;
  // Отсутствие ребра отмечаем значением int64_t, отличимым от любого допустимого веса int.
  constexpr Weight infinity = std::numeric_limits<Weight>::max();
  std::vector<std::vector<Weight>> matrix(vertices, std::vector<Weight>(vertices, infinity));
  for (const auto& edge : connections)
  {
    // Матрица для Прима хранит минимум из параллельных рёбер, включая нулевые веса.
    const Weight weight = std::min(matrix[edge.from][edge.to], Weight{edge.weight});
    matrix[edge.from][edge.to] = matrix[edge.to][edge.from] = weight;
  }
  // minEdge хранит цену подключения вершины, parent - откуда идёт это ребро.
  // included отделяет готовое дерево от ещё не присоединённых вершин.
  std::vector<Weight> minEdge(vertices, infinity);
  std::vector<int> parent(vertices, -1);
  std::vector<bool> included(vertices, false);
  // Нулевая цена старта позволяет выбрать корень первым, самого ребра к корню нет.
  minEdge[start] = 0;
  MstResult result;
  for (int step = 0; step < vertices; ++step)
  {
    // Полным просмотром выбираем самое дешёвое подключение к текущему дереву.
    int current = -1;
    for (int vertex = 0; vertex < vertices; ++vertex)
    {
      if (!included[vertex] && (current == -1 || minEdge[vertex] < minEdge[current]))
      {
        current = vertex;
      }
    }
    // Не осталось достижимой вершины: построить единый остов из данного старта нельзя.
    if (current == -1 || minEdge[current] == infinity)
    {
      throw std::domain_error("Граф не связный: MST не существует");
    }
    // Фиксируем вершину и выбранное для неё ребро, затем улучшаем подключения соседей.
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

// Объединяет компоненты концов ребра, меняя общий массив меток за O(V).
// False означает, что вершины уже связаны и добавление ребра создало бы цикл.
bool Graph::merge(std::vector<int>& labels, int from, int to)
{
  int keep = labels[from];
  int replace = labels[to];
  if (keep == replace)
  {
    return false;
  }
  // Представителем объединения оставляем меньшую метку для предсказуемой группировки.
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

// Строит MST, последовательно выбирая дешёвые рёбра между разными компонентами.
// Возвращает рёбра и вес, при отсутствии единого остова бросает domain_error.
MstResult Graph::kruskal() const
{
  if (vertices == 0)
  {
    throw std::domain_error("Для пустого графа MST не определено");
  }
  auto sorted = connections; // Сортируем копию, исходный порядок дорог сохраняется.
  // Концы ребра разрешают равенство весов, делая выбор воспроизводимым.
  std::sort(sorted.begin(), sorted.end(), [](const Edge& left, const Edge& right)
  {
    return std::tie(left.weight, left.from, left.to) < std::tie(right.weight, right.from, right.to);
  });
  std::vector<int> labels(vertices);
  // До выбора рёбер каждая вершина образует отдельную компоненту.
  std::iota(labels.begin(), labels.end(), 0);
  MstResult result;
  for (const auto& edge : sorted)
  {
    // Merge отсекает и циклы, и петли, так как их концы уже принадлежат одной компоненте.
    if (merge(labels, edge.from, edge.to))
    {
      result.edges.push_back(edge);
      result.weight += edge.weight;
      // Для связного дерева из V вершин достаточно V-1 рёбер, остальные не нужны.
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

// Возвращает группы номеров вершин, связанных путями, включая изолированные вершины.
std::vector<std::vector<int>> Graph::components() const
{
  std::vector<int> labels(vertices);
  std::iota(labels.begin(), labels.end(), 0);
  for (const auto& edge : connections)
  {
    merge(labels, edge.from, edge.to);
  }
  // После объединения по всем рёбрам одинаковые метки обозначают одну компоненту.
  std::vector<std::vector<int>> groups(vertices);
  for (int vertex = 0; vertex < vertices; ++vertex)
  {
    groups[labels[vertex]].push_back(vertex);
  }
  // Метки равны минимальным номерам компонент, пустые промежутки удаляются.
  std::erase_if(groups, [](const auto& group) { return group.empty(); });
  return groups;
}
