#include "HashTable.h"

#include <cstdint>
#include <limits>
#include <ostream>
#include <stdexcept>

namespace
{
  // Возвращает первое простое число не меньше requested, сохраняя ограничение ёмкости типом int.
  // Простая ёмкость нужна для гарантии вставки при квадратичных пробах и загрузке менее 50%.
  std::size_t nextPrime(int requested)
  {
    if (requested < 3)
    {
      throw std::invalid_argument("Ёмкость должна быть не меньше 3");
    }
    for (int candidate = requested;; ++candidate)
    {
      bool prime = true;
      // Достаточно делителей до корня. Деление вместо divisor*divisor исключает переполнение.
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
      // До увеличения candidate проверяем предел, чтобы не переполнить знаковый int.
      if (candidate == std::numeric_limits<int>::max())
      {
        throw std::length_error("Не удалось выбрать ёмкость");
      }
    }
  }
}

// Выделяет фиксированный массив простой ёмкости. Entry по умолчанию имеет состояние Empty.
HashTable::HashTable(int requestedCapacity) : slots(nextPrime(requestedCapacity)) {}

// Преобразует целый ключ в начальный индекс ячейки, укладывающийся в ёмкость таблицы.
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
  // Остаток переводит перемешанное число в диапазон индексов массива.
  return value % slots.size();
}

// Вычисляет очередную квадратичную пробу (base + step^2) mod capacity.
// При step == 0 проверяется исходная хеш-позиция.
std::size_t HashTable::probe(std::size_t base, std::size_t step) const
{
  // Ёмкость ограничена int. Произведение в uint64_t не переполняется даже на 32-битной платформе.
  return static_cast<std::size_t>((base + static_cast<std::uint64_t>(step) * step) % slots.size());
}

// Идёт по той же последовательности проб, что вставка. Возвращает индекс или slots.size().
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
    // Содержимое удалённой ячейки уже не является действующей парой ключ-значение.
    if (entry.state == State::Occupied && entry.key == key)
    {
      return index;
    }
    // Deleted пропускается: за ним может находиться вытесненный коллизией ключ.
  }
  return slots.size(); // Ограничение числа проб исключает бесконечный цикл.
}

// Добавляет пару или заменяет значение существующего ключа. Расширение таблицы не выполняется.
void HashTable::insert(int key, int value)
{
  // Сначала ищем ключ до конца его цепочки, иначе можно создать дубликат за меткой Deleted.
  const std::size_t existing = findIndex(key);
  if (existing != slots.size())
  {
    slots[existing].value = value; // Обновление разрешено и при предельной загрузке.
    return;
  }
  // Ограничиваем число занятых ячеек: квадратичные пробы могут посетить не весь массив.
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
      // Только новая пара меняет число элементов, обновление существующей пары его сохраняет.
      ++count;
      return;
    }
  }
  throw std::overflow_error("В последовательности проб нет свободной ячейки");
}

// Возвращает невладеющий указатель на найденное значение или nullptr.
// После удаления этой пары указатель больше нельзя использовать как результат поиска.
const int* HashTable::find(int key) const
{
  const std::size_t index = findIndex(key);
  return index == slots.size() ? nullptr : &slots[index].value;
}

// Помечает найденную ячейку удалённой. True означает удаление, false - отсутствие ключа.
bool HashTable::remove(int key)
{
  const std::size_t index = findIndex(key);
  if (index == slots.size())
  {
    return false;
  }
  // Empty разорвал бы цепочку поиска других ключей, поэтому сохраняем отдельную метку Deleted.
  slots[index].state = State::Deleted;
  --count;
  return true;
}

// Показывает все физические ячейки, включая Empty и Deleted, для разбора коллизий.
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
