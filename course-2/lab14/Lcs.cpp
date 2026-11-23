#include "Lcs.h"

#include <algorithm>
#include <optional>
#include <stdexcept>
#include <utility>

namespace
{
  using Sequences = std::set<std::string>;
  using Memo = std::vector<std::vector<std::optional<Sequences>>>;
  using Table = std::vector<std::vector<std::size_t>>;

  void validate(std::string_view text)
  {
    for (unsigned char symbol : text)
    {
      if (symbol < ' ' || symbol > '~')
      {
        throw std::invalid_argument("LCS принимает только печатные символы ASCII");
      }
    }
  }

  const Sequences& restore(std::string_view first, std::string_view second,
                           const Table& table, std::size_t i, std::size_t j, Memo& memo)
  {
    auto& result = memo[i][j];
    if (result)
    {
      return *result;
    }
    result.emplace(); // Теперь у данного состояния есть собственное множество ответов.
    if (table[i][j] == 0)
    {
      result->insert(""); // Пустая строка - основа для дописывания совпавших символов.
    }
    else if (first[i - 1] == second[j - 1])
    {
      for (const auto& prefix : restore(first, second, table, i - 1, j - 1, memo))
      {
        result->insert(prefix + first[i - 1]);
      }
    }
    else
    {
      // При равных оптимумах нужны обе ветви. set удаляет совпавшие строки разных ветвей.
      if (table[i - 1][j] == table[i][j])
      {
        const auto& upper = restore(first, second, table, i - 1, j, memo);
        result->insert(upper.begin(), upper.end());
      }
      if (table[i][j - 1] == table[i][j])
      {
        const auto& left = restore(first, second, table, i, j - 1, memo);
        result->insert(left.begin(), left.end());
      }
    }
    return *result;
  }
}

LcsResult findAllLcs(std::string_view first, std::string_view second)
{
  validate(first);
  validate(second);
  const auto rows = first.size() + 1;
  const auto columns = second.size() + 1;
  Table table(rows, std::vector<std::size_t>(columns, 0));
  for (std::size_t i = 1; i < rows; ++i)
  {
    for (std::size_t j = 1; j < columns; ++j)
    {
      // Состояния относятся к префиксам длины i и j, символы имеют индексы i-1 и j-1.
      table[i][j] = first[i - 1] == second[j - 1]
        ? table[i - 1][j - 1] + 1
        : std::max(table[i - 1][j], table[i][j - 1]);
    }
  }
  Memo memo(rows, std::vector<std::optional<Sequences>>(columns));
  restore(first, second, table, first.size(), second.size(), memo);
  // Переносим готовое множество из кеша. Остальные промежуточные множества освобождаются.
  return {table.back().back(), std::move(*memo.back().back()), std::move(table)};
}
