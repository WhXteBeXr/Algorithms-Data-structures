#pragma once
#include <chrono>

// Шаблонная функция измерения времени выполнения произвольного
// вызываемого объекта (лямбды)
template <typename Func>
long long measureTime(Func func) {
  auto start = std::chrono::high_resolution_clock::now();
  func();
  auto end = std::chrono::high_resolution_clock::now();
  return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}