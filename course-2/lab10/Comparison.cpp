#include "Comparison.h"
#include "SearchTree.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <iomanip>
#include <numeric>
#include <ostream>
#include <random>
#include <span>
#include <stdexcept>
#include <vector>

namespace
{
  using Clock = std::chrono::steady_clock;
  struct Measurement
  {
    double insertion;
    double search;
    int height;
    std::size_t hits;
  };

  Measurement measure(SearchTree::Kind kind, std::span<const int> values, std::span<const int> queries)
  {
    SearchTree tree(kind);
    std::size_t inserted = 0;
    const auto start = Clock::now();
    for (int key : values)
    {
      inserted += tree.insert(key);
    }
    const auto built = Clock::now();
    std::size_t hits = 0;
    for (int key : queries)
    {
      hits += tree.search(key);
    }
    const auto searched = Clock::now();
    // Контроль результатов вне интервалов, вычисление высоты и разрушение тоже не измеряются.
    if (inserted != values.size() || hits != queries.size() / 2)
    {
      throw std::logic_error("Несогласованные результаты сравнения BST и AVL");
    }
    return {std::chrono::duration<double, std::micro>(built - start).count(),
            std::chrono::duration<double, std::micro>(searched - built).count(), tree.height(), hits};
  }

  void printMedian(std::ostream& out, const char* name, const std::array<Measurement, 3>& runs)
  {
    std::array<double, 3> insertions;
    std::array<double, 3> searches;
    for (std::size_t i = 0; i < runs.size(); ++i)
    {
      insertions[i] = runs[i].insertion;
      searches[i] = runs[i].search;
    }
    std::sort(insertions.begin(), insertions.end());
    std::sort(searches.begin(), searches.end());
    out << name << ": h=" << runs[0].height << ", вставка=" << insertions[1]
        << " мкс, поиск=" << searches[1] << " мкс, найдено=" << runs[0].hits << '\n';
  }
}

void runComparison(std::ostream& out)
{
  out << "Часть 3. Сравнение BST и AVL\n"
      << "Поиск: 4000 запросов, 2000 попаданий и 2000 промахов. Медианы 3 запусков.\n";
  out << std::fixed << std::setprecision(2);
  for (int n : {1000, 5000, 10000})
  {
    std::vector<int> values(n);
    std::iota(values.begin(), values.end(), 0);
    std::mt19937 generator(2026 + n);
    std::shuffle(values.begin(), values.end(), generator);
    // Запросы не зависят от порядка вставки. Одинаковая смесь для обеих форм дерева.
    std::uniform_int_distribution<int> distribution(0, n - 1);
    std::vector<int> queries;
    for (int i = 0; i < 2000; ++i)
    {
      queries.push_back(distribution(generator));
      queries.push_back(n + distribution(generator));
    }
    std::shuffle(queries.begin(), queries.end(), generator);
    for (bool sorted : {false, true})
    {
      if (sorted)
      {
        std::sort(values.begin(), values.end());
      }
      std::array<Measurement, 3> bst;
      std::array<Measurement, 3> avl;
      for (std::size_t i = 0; i < bst.size(); ++i)
      {
        // Чередуем порядок структур, чтобы одна не измерялась всегда первой.
        if (i % 2 == 0)
        {
          bst[i] = measure(SearchTree::Kind::BST, values, queries);
          avl[i] = measure(SearchTree::Kind::AVL, values, queries);
        }
        else
        {
          avl[i] = measure(SearchTree::Kind::AVL, values, queries);
          bst[i] = measure(SearchTree::Kind::BST, values, queries);
        }
      }
      out << "N=" << n << ", " << (sorted ? "отсортированные данные" : "случайные данные") << '\n';
      printMedian(out, "BST", bst);
      printMedian(out, "AVL", avl);
    }
  }
}
