#include "BinarySearch.h"
#include "Comparison.h"
#include "HashTable.h"

#include <exception>
#include <iostream>
#include <limits>

namespace
{
  template <typename Action>
  void showError(const char* label, Action action)
  {
    try
    {
      action();
    }
    catch (const std::exception& error)
    {
      std::cout << label << ": " << error.what() << '\n';
    }
  }

  void showValue(const HashTable& table, int key)
  {
    std::cout << "find(" << key << "): ";
    if (const int* value = table.find(key))
    {
      std::cout << *value;
    }
    else
    {
      std::cout << "не найден";
    }
    std::cout << '\n';
  }
}

int main()
{
  std::cout << std::boolalpha;
  try
  {
    std::cout << "=== Часть 1. Первое вхождение ===\n";
    const int values[] = {-4, 1, 1, 1, 5, 9};
    for (int key : {-4, 1, 9, 3, -10, 10})
    {
      std::cout << "Ключ " << key << ": индекс " << firstOccurrence(values, key) << '\n';
    }
    const int same[] = {7, 7, 7};
    const int single[] = {42};
    std::cout << "Все значения одинаковы, первое 7: " << firstOccurrence(same, 7) << '\n'
              << "Один элемент 42: " << firstOccurrence(single, 42) << '\n'
              << "Пустой массив: " << firstOccurrence({}, 42) << '\n';

    std::cout << "\n=== Часть 2. Квадратичное пробирование ===\n";
    HashTable table(7);
    // Для выбранной хеш-функции все четыре ключа имеют начальный бакет 0.
    for (int key : {0, 6, 8})
    {
      table.insert(key, key + 10);
    }
    table.print(std::cout);
    table.insert(8, 300); // Обновление при заполненном лимите не добавляет новую запись.
    showValue(table, 8);
    std::cout << "Размер после обновления: " << table.size() << '\n';
    showError("Четвёртая запись при ёмкости 7", [&] { table.insert(9, 40); });
    std::cout << "remove(0): " << table.remove(0) << '\n';
    table.print(std::cout);
    showValue(table, 8); // Поиск проходит через deleted и находит элемент дальше.
    table.insert(8, 301); // Не создаём второй ключ 8 за удалённой ячейкой.
    table.insert(9, 40); // Повторно используем освобождённое место.
    table.print(std::cout);
    showValue(table, 8);
    std::cout << "remove(0) повторно: " << table.remove(0) << ", remove(99): " << table.remove(99) << '\n';
    showValue(table, 99);

    HashTable boundaries(8); // Размер округляется до 11.
    for (int key : {std::numeric_limits<int>::min(), -1, 0, std::numeric_limits<int>::max()})
    {
      boundaries.insert(key, key);
    }
    std::cout << "Округлённая ёмкость: " << boundaries.capacity() << '\n';
    showValue(boundaries, std::numeric_limits<int>::min());
    showValue(boundaries, std::numeric_limits<int>::max());
    HashTable empty;
    std::cout << "Пустая таблица: размер " << empty.size() << ", remove(1): " << empty.remove(1) << '\n';
    showValue(empty, 1);
    showError("Ёмкость 0", [] { HashTable invalid(0); });
    compareSearch(std::cout);
  }
  catch (const std::exception& error)
  {
    std::cerr << "Ошибка выполнения: " << error.what() << '\n';
    return 1;
  }
}
