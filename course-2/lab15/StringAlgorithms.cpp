#include "StringAlgorithms.h"

#include <algorithm>
#include <stdexcept>
#include <string>

namespace
{
  // Проверяет печатный ASCII, чтобы позиции байтов совпадали с позициями символов.
  void validate(std::string_view text)
  {
    for (unsigned char symbol : text)
    {
      if (symbol < ' ' || symbol > '~')
      {
        throw std::invalid_argument("Поддерживаются только печатные символы ASCII");
      }
    }
  }
}

// Для каждой позиции возвращает длину наибольшего собственного префикса, совпавшего с суффиксом.
// Собственный префикс короче всей рассматриваемой части образца. Построение занимает O(M).
std::vector<std::size_t> prefixFunction(std::string_view pattern)
{
  validate(pattern);
  std::vector<std::size_t> prefix(pattern.size(), 0);
  for (std::size_t i = 1; i < pattern.size(); ++i)
  {
    // Пробуем продлить границу предыдущей части новым символом pattern[i].
    auto matched = prefix[i - 1];
    // После несовпадения ищем более короткую границу уже совпавшего префикса.
    while (matched > 0 && pattern[i] != pattern[matched])
    {
      matched = prefix[matched - 1];
    }
    if (pattern[i] == pattern[matched])
    {
      ++matched;
    }
    // Сохраняем границу для части pattern[0..i], она понадобится следующим позициям и КМП.
    prefix[i] = matched;
  }
  return prefix;
}

// Возвращает все начальные индексы образца в тексте, включая перекрывающиеся совпадения.
// Время O(N+M), текст и образец передаются без копирования и не изменяются.
std::vector<std::size_t> kmpSearch(std::string_view text, std::string_view pattern)
{
  validate(text);
  validate(pattern);
  // По принятому соглашению пустой образец не даёт вхождений, слишком длинный тоже не подходит.
  if (pattern.empty() || pattern.size() > text.size())
  {
    return {};
  }
  const auto prefix = prefixFunction(pattern);
  std::vector<std::size_t> positions;
  // matched - длина префикса образца, совпавшего с суффиксом уже просмотренного текста.
  std::size_t matched = 0;
  for (std::size_t i = 0; i < text.size(); ++i)
  {
    // Индекс текста не откатывается, меняется только длина совпавшей части образца.
    while (matched > 0 && text[i] != pattern[matched])
    {
      matched = prefix[matched - 1];
    }
    if (text[i] == pattern[matched])
    {
      ++matched;
    }
    if (matched == pattern.size())
    {
      // i - конец полного совпадения, вычитание длины даёт его начальный индекс.
      positions.push_back(i + 1 - pattern.size());
      // Сохраняем совпавший суффикс, чтобы найти и перекрывающиеся вхождения.
      matched = prefix[matched - 1];
    }
  }
  return positions;
}

// Проверяет совпадение наборов символов с учётом повторов, регистра, пробелов и знаков.
bool areAnagrams(std::string_view first, std::string_view second)
{
  validate(first);
  validate(second);
  if (first.size() != second.size())
  {
    return false;
  }
  // string владеют копиями. Сортировка не меняет исходный текст и образец.
  std::string left(first);
  std::string right(second);
  std::sort(left.begin(), left.end());
  std::sort(right.begin(), right.end());
  // У анаграмм отсортированные копии равны. Порядок символов исходных строк не важен.
  return left == right;
}
