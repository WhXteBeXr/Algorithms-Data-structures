#include "Recursion.h"

#include <array>
#include <stdexcept>
#include <utility>

namespace
{
  // Проверяет индекс до вычислений: отрицательные значения запрещены,
  // maximum задаёт последний аргумент, результат которого помещается в uint64_t.
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

  // Возвращает F(n), сохраняя решения подзадач в общем для текущего расчёта кэше.
  std::uint64_t fibonacciCached(int n, std::array<std::uint64_t, 94>& cache)
  {
    // F(0) и F(1) известны сразу и останавливают углубление рекурсии.
    if (n < 2)
    {
      return static_cast<std::uint64_t>(n);
    }
    // F(n) для n >= 2 положительно, поэтому ноль обозначает отсутствие результата.
    if (cache[n] == 0)
    {
      // Каждая подзадача вычисляется один раз, повторные обращения читают кэш.
      cache[n] = fibonacciCached(n - 1, cache) + fibonacciCached(n - 2, cache);
    }
    return cache[n];
  }

  // Проверяет текст перед рекурсией, не копируя и не изменяя исходную строку.
  void validateText(std::string_view text)
  {
    // Ограничиваем глубину вызовов и не пытаемся переставлять байты UTF-8 как символы.
    if (text.size() > 4096)
    {
      throw std::length_error("Допускается не более 4096 символов");
    }
    // Беззнаковый байт позволяет однозначно обнаружить значения вне ASCII.
    for (unsigned char symbol : text)
    {
      if (symbol > 127)
      {
        throw std::invalid_argument("Ожидается строка ASCII");
      }
    }
  }

  // Разворачивает на месте полуоткрытый диапазон [left, right) исходной строки.
  void reverseRange(std::string& text, std::size_t left, std::size_t right)
  {
    // right - граница после последнего символа, поэтому пустая строка безопасна.
    if (right - left < 2)
    {
      return;
    }
    // Крайние символы занимают итоговые места, дальше обрабатывается только середина.
    std::swap(text[left], text[right - 1]);
    reverseRange(text, left + 1, right - 1);
  }

  // Сравнивает края текущего представления, затем проверяет оставшуюся середину.
  bool palindromeRange(std::string_view text)
  {
    // Пустая строка и один символ являются палиндромами без дополнительных сравнений.
    if (text.size() < 2)
    {
      return true;
    }
    // При несовпадении дальнейший вызов не нужен. substr у string_view не копирует текст.
    return text.front() == text.back() && palindromeRange(text.substr(1, text.size() - 2));
  }
}

// Возвращает n! рекурсивно. Допустимы 0..20, большие результаты переполнят uint64_t.
std::uint64_t factorialRecursive(int n)
{
  validateIndex(n, 20);
  // Значения 0! и 1! равны 1, это база рекурсии.
  if (n < 2)
  {
    return 1;
  }
  // Умножение выполняется при возврате, когда меньшая подзадача уже решена.
  return static_cast<std::uint64_t>(n) * factorialRecursive(n - 1);
}

// Вычисляет тот же факториал циклом, используя O(1) дополнительной памяти.
std::uint64_t factorialIterative(int n)
{
  validateIndex(n, 20);
  // Накапливаем произведение 2..n, для 0 и 1 цикл не выполняется.
  std::uint64_t result = 1;
  for (int i = 2; i <= n; ++i)
  {
    result *= static_cast<std::uint64_t>(i);
  }
  return result;
}

// Возвращает F(n) для 0..93 через рекурсию с мемоизацией, время расчёта O(n).
std::uint64_t fibonacciRecursive(int n)
{
  validateIndex(n, 93);
  // Каждый внешний вызов получает свой нулевой кэш, общих изменяемых данных нет.
  std::array<std::uint64_t, 94> cache{};
  return fibonacciCached(n, cache);
}

// Возвращает F(n) без рекурсивного стека, последовательно обновляя пару соседних чисел.
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

// Принимает собственную строку по значению, разворачивает её и возвращает результат.
// Переданная вызывающим кодом lvalue-строка остаётся неизменной.
std::string reverseString(std::string text)
{
  validateText(text);
  // Передаём весь диапазон, включая безопасный случай пустого текста.
  reverseRange(text, 0, text.size());
  return text;
}

// Проверяет ASCII-строку без копирования. Регистр, пробелы и знаки учитываются буквально.
bool isPalindrome(std::string_view text)
{
  validateText(text);
  return palindromeRange(text);
}
