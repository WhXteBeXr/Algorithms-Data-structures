#include "HashTable.h"

#include <cstdint>
#include <limits>
#include <ostream>
#include <stdexcept>

namespace
{
  std::size_t nextPrime(int requested)
  {
    if (requested < 3)
    {
      throw std::invalid_argument("Ёмкость должна быть не меньше 3");
    }
    for (int candidate = requested;; ++candidate)
    {
      bool prime = true;
      for (int divisor = 2; divisor <= candidate / divisor; ++divisor)
      {
        if (candidate % divisor == 0)
        {
          prime = false;
          break;
        }
      }
      if (prime)
      {
        return static_cast<std::size_t>(candidate);
      }
      if (candidate == std::numeric_limits<int>::max())
      {
        throw std::length_error("Не удалось выбрать ёмкость");
      }
    }
  }
}

HashTable::HashTable(int requestedCapacity) : slots(nextPrime(requestedCapacity)) {}

std::size_t HashTable::hashKey(int key) const
{
  // Перемешивание битов в беззнаковой арифметике корректно и для отрицательных ключей.
  std::uint32_t value = static_cast<std::uint32_t>(key);
  value ^= value >> 16;
  // Переполнение uint32_t определено как вычисление по модулю 2^32.
  value *= 0x7feb352dU;
  value ^= value >> 15;
  value *= 0x846ca68bU;
  value ^= value >> 16;
  return value % slots.size();
}

std::size_t HashTable::probe(std::size_t base, std::size_t step) const
{
  // Ёмкость ограничена int. Произведение в uint64_t не переполняется даже на 32-битной платформе.
  return static_cast<std::size_t>((base + static_cast<std::uint64_t>(step) * step) % slots.size());
}

std::size_t HashTable::findIndex(int key) const
{
  const std::size_t base = hashKey(key);
  for (std::size_t step = 0; step < slots.size(); ++step)
  {
    const std::size_t index = probe(base, step);
    const Entry& entry = slots[index];
    if (entry.state == State::Empty)
    {
      return slots.size(); // Настоящая пустая ячейка завершает поиск.
    }
    if (entry.state == State::Occupied && entry.key == key)
    {
      return index;
    }
    // Deleted пропускается: за ним может находиться вытесненный коллизией ключ.
  }
  return slots.size(); // Ограничение числа проб исключает бесконечный цикл.
}

void HashTable::insert(int key, int value)
{
  const std::size_t existing = findIndex(key);
  if (existing != slots.size())
  {
    slots[existing].value = value; // Обновление разрешено и при предельной загрузке.
    return;
  }
  if (count >= slots.size() / 2)
  {
    throw std::overflow_error("Превышен лимит загрузки 50%, нужна большая таблица");
  }
  const std::size_t base = hashKey(key);
  for (std::size_t step = 0; step < slots.size(); ++step)
  {
    Entry& entry = slots[probe(base, step)];
    if (entry.state != State::Occupied)
    {
      // Повторный ключ уже исключён поиском, удалённое место можно сразу использовать.
      entry = {key, value, State::Occupied};
      ++count;
      return;
    }
  }
  throw std::overflow_error("В последовательности проб нет свободной ячейки");
}

const int* HashTable::find(int key) const
{
  const std::size_t index = findIndex(key);
  return index == slots.size() ? nullptr : &slots[index].value;
}

bool HashTable::remove(int key)
{
  const std::size_t index = findIndex(key);
  if (index == slots.size())
  {
    return false;
  }
  slots[index].state = State::Deleted;
  --count;
  return true;
}

void HashTable::print(std::ostream& out) const
{
  for (std::size_t i = 0; i < slots.size(); ++i)
  {
    out << i << ": ";
    if (slots[i].state == State::Occupied)
    {
      out << slots[i].key << " -> " << slots[i].value;
    }
    else
    {
      out << (slots[i].state == State::Empty ? "empty" : "deleted");
    }
    out << '\n';
  }
}
