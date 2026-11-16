#pragma once

#include <cstddef>
#include <iosfwd>
#include <vector>

class HashTable
{
public:
  // Запрошенный размер >= 3 округляется вверх до простого числа.
  explicit HashTable(int requestedCapacity = 11);
  HashTable(const HashTable&) = delete;
  HashTable& operator=(const HashTable&) = delete;

  // Без автоматического расширения, не более floor(capacity/2) разных ключей.
  void insert(int key, int value);
  // nullptr означает отсутствие ключа. Указатель не владеет значением.
  const int* find(int key) const;
  bool remove(int key);
  std::size_t size() const { return count; }
  std::size_t capacity() const { return slots.size(); }
  void print(std::ostream& out) const;

private:
  enum class State { Empty, Occupied, Deleted };
  struct Entry
  {
    int key = 0;
    int value = 0;
    State state = State::Empty;
  };
  std::vector<Entry> slots;
  std::size_t count = 0;

  std::size_t hashKey(int key) const;
  std::size_t probe(std::size_t base, std::size_t step) const;
  std::size_t findIndex(int key) const;
};
