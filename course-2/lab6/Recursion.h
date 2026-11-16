#pragma once

#include <cstdint>
#include <string>
#include <string_view>

// Для uint64_t поддерживаются 0..20 для факториала и 0..93 для Фибоначчи.
std::uint64_t factorialRecursive(int n);
std::uint64_t factorialIterative(int n);
std::uint64_t fibonacciRecursive(int n);
std::uint64_t fibonacciIterative(int n);
// Строки ASCII длиной до 4096. Регистр и пробелы учитываются.
std::string reverseString(std::string text);
bool isPalindrome(std::string_view text);
