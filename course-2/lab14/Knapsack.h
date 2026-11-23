#pragma once

#include <cstddef>
#include <cstdint>
#include <span>
#include <vector>

struct Item
{
  int weight;
  int value;
};

struct KnapsackResult
{
  std::int64_t maxValue;
  std::vector<std::size_t> selected;
  std::vector<std::vector<std::int64_t>> table;
};

// Рюкзак 0/1: каждый предмет доступен один раз. Нулевой вес допустим.
KnapsackResult solveKnapsack(std::span<const Item> items, int capacity);
