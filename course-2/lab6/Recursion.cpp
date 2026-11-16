#include "Recursion.h"

#include <array>
#include <stdexcept>
#include <utility>

namespace
{
  void validateIndex(int n, int maximum)
  {
    if (n < 0)
    {
      throw std::invalid_argument("Аргумент не может быть отрицательным");
    }
    if (n > maximum)
    {
      throw std::overflow_error("Результат не помещается в uint64_t");
    }
  }

  std::uint64_t fibonacciCached(int n, std::array<std::uint64_t, 94>& cache)
  {
    if (n < 2)
    {
      return static_cast<std::uint64_t>(n);
    }
    // F(n) для n >= 2 положительно, поэтому ноль обозначает отсутствие результата.
    if (cache[n] == 0)
    {
      cache[n] = fibonacciCached(n - 1, cache) + fibonacciCached(n - 2, cache);
    }
    return cache[n];
  }

  void validateText(std::string_view text)
  {
    // Ограничиваем глубину вызовов и не пытаемся переставлять байты UTF-8 как символы.
    if (text.size() > 4096)
    {
      throw std::length_error("Допускается не более 4096 символов");
    }
    for (unsigned char symbol : text)
    {
      if (symbol > 127)
      {
        throw std::invalid_argument("Ожидается строка ASCII");
      }
    }
  }

  void reverseRange(std::string& text, std::size_t left, std::size_t right)
  {
    // right - граница после последнего символа, поэтому пустая строка безопасна.
    if (right - left < 2)
    {
      return;
    }
    std::swap(text[left], text[right - 1]);
    reverseRange(text, left + 1, right - 1);
  }

  bool palindromeRange(std::string_view text)
  {
    if (text.size() < 2)
    {
      return true;
    }
    // При несовпадении дальнейший вызов не нужен. substr у string_view не копирует текст.
    return text.front() == text.back() && palindromeRange(text.substr(1, text.size() - 2));
  }
}

std::uint64_t factorialRecursive(int n)
{
  validateIndex(n, 20);
  if (n < 2)
  {
    return 1;
  }
  // Умножение выполняется при возврате, когда меньшая подзадача уже решена.
  return static_cast<std::uint64_t>(n) * factorialRecursive(n - 1);
}

std::uint64_t factorialIterative(int n)
{
  validateIndex(n, 20);
  std::uint64_t result = 1;
  for (int i = 2; i <= n; ++i)
  {
    result *= static_cast<std::uint64_t>(i);
  }
  return result;
}

std::uint64_t fibonacciRecursive(int n)
{
  validateIndex(n, 93);
  // Каждый внешний вызов получает свой нулевой кэш, общих изменяемых данных нет.
  std::array<std::uint64_t, 94> cache{};
  return fibonacciCached(n, cache);
}

std::uint64_t fibonacciIterative(int n)
{
  validateIndex(n, 93);
  if (n < 2)
  {
    return static_cast<std::uint64_t>(n);
  }
  // В цикле достаточно двух предыдущих чисел, вся последовательность не хранится.
  std::uint64_t previous = 0;
  std::uint64_t current = 1;
  for (int i = 2; i <= n; ++i)
  {
    const std::uint64_t next = previous + current;
    previous = current;
    current = next; // На последнем шаге вычисляем F(n), а не переполняющий F(n+1).
  }
  return current;
}

std::string reverseString(std::string text)
{
  validateText(text);
  reverseRange(text, 0, text.size());
  return text;
}

bool isPalindrome(std::string_view text)
{
  validateText(text);
  return palindromeRange(text);
}
