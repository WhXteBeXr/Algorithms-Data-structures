#include "AmortizedAnalysis.h"
#include "Vector.h"

#include <algorithm>
#include <exception>
#include <iostream>
#include <iterator>
#include <limits>
#include <memory>
#include <string>
#include <utility>

namespace
{
  template <typename T>
  void printVector(const char* label, const Vector<T>& values)
  {
    std::cout << label;
    values.print(std::cout);
    std::cout << ", размер = " << values.size() << ", ёмкость = " << values.capacity() << '\n';
  }

  // Общая обёртка только для демонстрации ожидаемых ошибок.
  // После catch следующий пример продолжает выполняться.
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

  void demonstrateConstructionAndGrowth()
  {
    std::cout << "=== Конструкторы и рост ёмкости ===\n";
    Vector<int> empty;
    Vector<int> zeros(3);
    Vector<int> filled(4, 7);
    printVector("По умолчанию: ", empty);
    printVector("Три нуля: ", zeros);
    printVector("Четыре семёрки: ", filled);

    Vector<int> values;
    for (int value = 10; value <= 50; value += 10)
    {
      values.push_back(value);
      printVector("После push_back: ", values);
    }
    values[0] = 100; // Неконстантный operator[] разрешает изменять элемент.
    const Vector<int>& view = values;
    std::cout << "Чтение через const operator[]: " << view[0] << '\n';

    Vector<std::string> words;
    words.push_back("строка");
    words.push_back(words[0]); // Расширение должно сохранить аргумент из старого массива.
    printVector("push_back своего элемента: ", words);
    words.resize(4, words[0]); // То же условие важно при заполнении новых ячеек.
    printVector("resize со своим элементом: ", words);
  }

  void demonstrateModificationAndLifetime()
  {
    std::cout << "\n=== Изменение размера и время жизни ===\n";
    Vector<int> values(3, 5);
    values.reserve(8);
    printVector("reserve(8), размер прежний: ", values);
    values.reserve(2); // reserve никогда не уменьшает ёмкость.
    printVector("reserve(2), ёмкость прежняя: ", values);
    values.resize(5, 9); // Рост внутри уже выделенной памяти.
    printVector("resize(5, 9): ", values);
    values.pop_back();
    values.resize(2);
    printVector("После pop_back и resize(2): ", values);
    values.clear();
    std::cout << "После clear: пуст = " << values.empty() << ", ёмкость = " << values.capacity() << '\n';
    values.push_back(42);
    printVector("Повторное использование: ", values);

    // Три shared_ptr владеют одним числом. weak_ptr наблюдает за его жизнью,
    // но сам не удерживает объект. Удаление ячеек должно уменьшать число владельцев.
    Vector<std::shared_ptr<int>> owners(3, std::make_shared<int>(7));
    std::weak_ptr<int> observer = owners[0];
    std::cout << "Владельцев ресурса до удаления: " << observer.use_count() << '\n';
    owners.pop_back();
    std::cout << "После pop_back: " << observer.use_count() << '\n';
    owners.resize(1);
    std::cout << "После resize(1): " << observer.use_count() << '\n';
    owners.clear();
    std::cout << "После clear ресурс уничтожен: " << observer.expired() << '\n';
  }

  void demonstrateRuleOfFive()
  {
    std::cout << "\n=== Правило пяти, обмен и счётчик копирований ===\n";
    Vector<int> source;
    for (int value : {1, 2, 3})
    {
      source.push_back(value);
    }
    std::cout << "Копирований при росте source: " << source.getCopyCount() << '\n';

    Vector<int> copy(source); // Новый независимый буфер.
    copy[0] = -1;
    printVector("source: ", source);
    printVector("Изменённая копия: ", copy);
    Vector<int> assigned(2, 99);
    assigned = source;
    assigned[2] = 100;
    printVector("Копирующее присваивание: ", assigned);

    Vector<int>& alias = source;
    source = alias; // Проверка адресов защищает от самоприсваивания.
    printVector("Самоприсваивание: ", source);
    Vector<int> moved(std::move(source));
    printVector("Перемещённый массив: ", moved);
    printVector("Источник после перемещения: ", source);
    std::cout << "История переместилась: moved = " << moved.getCopyCount()
              << ", source = " << source.getCopyCount() << '\n';
    source.push_back(77); // Перемещённый объект остаётся пригодным к использованию.

    Vector<int> target(2, 8);
    target = std::move(assigned); // Старые элементы target уничтожаются.
    printVector("Перемещающее присваивание: ", target);
    printVector("Источник присваивания: ", assigned);
    Vector<int>& sameTarget = target;
    target = std::move(sameTarget);
    printVector("Самоперемещение: ", target);

    moved.swap(copy); // Содержимое и история меняются вместе, элементы не копируются.
    printVector("После swap, moved: ", moved);
    printVector("После swap, copy: ", copy);
    std::cout << "История после swap: moved = " << moved.getCopyCount()
              << ", copy = " << copy.getCopyCount() << '\n';
    copy.resetCopyCount();
    std::cout << "После resetCopyCount: " << copy.getCopyCount() << '\n';
  }

  void demonstrateArithmeticAndErrors()
  {
    std::cout << "\n=== Числовые методы и граничные случаи ===\n";
    Vector<int> integers;
    for (int value : {1, 2, 3, 4, 5})
    {
      integers.push_back(value);
    }
    std::cout << "Для [1, 2, 3, 4, 5]: сумма = " << integers.sum()
              << ", среднее = " << integers.average() << '\n';
    Vector<double> fractions;
    fractions.push_back(1.5);
    fractions.push_back(2.5);
    std::cout << "Для [1.5, 2.5]: сумма = " << fractions.sum() << ", среднее = " << fractions.average() << '\n';
    Vector<int> large(2, std::numeric_limits<int>::max());
    std::cout << "Сумма двух INT_MAX: " << std::fixed << large.sum() << std::defaultfloat << '\n';

    Vector<int> empty;
    std::cout << "Сумма пустого массива: " << empty.sum() << '\n';
    showError("Vector(-1)", [] { Vector<int> invalid(-1); });
    showError("Индекс -1", [&] { integers[-1]; });
    showError("Индекс size()", [&] { integers[static_cast<std::ptrdiff_t>(integers.size())]; });
    showError("pop_back пустого массива", [&] { empty.pop_back(); });
    showError("average пустого массива", [&] { empty.average(); });
    showError("reserve(-1)", [&] { integers.reserve(-1); });
    showError("resize(-1)", [&] { integers.resize(-1); });
    printVector("После ошибочных операций: ", integers);
  }

  void demonstrateIterators()
  {
    std::cout << "\n=== Прямые и обратные итераторы ===\n";
    Vector<int> values;
    for (int value : {5, 1, 4, 2, 3})
    {
      values.push_back(value);
    }
    std::cout << "range-based for:";
    for (int& value : values)
    {
      value *= 2;
      std::cout << ' ' << value;
    }
    std::cout << '\n';
    const Vector<int>& view = values;
    std::cout << "const begin/end: ";
    std::copy(view.begin(), view.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << "\nconst rbegin/rend: ";
    std::copy(view.rbegin(), view.rend(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << '\n';

    auto it = values.rbegin();
    std::cout << "base(rbegin) == end: " << (it.base() == values.end()) << '\n';
    std::cout << "*it++ = " << *it++ << ", затем *it = " << *it << '\n';
    std::cout << "*++it = " << *++it << ", *--it = " << *--it << '\n';
    const auto previous = it--;
    std::cout << "После it--: прежний элемент = " << *previous << ", текущий = " << *it << '\n';
    auto first = values.rend();
    std::cout << "*--rend = " << *--first << '\n';
    Vector<int>::const_reverse_iterator readOnly = values.rbegin();
    std::cout << "Сравнение const и обычного reverse_iterator: " << (readOnly == values.rbegin()) << '\n';

    for (auto reverse = values.rbegin(); reverse != values.rend(); ++reverse)
    {
      *reverse += 1; // Неконстантный обратный итератор позволяет менять исходный массив.
    }
    printVector("После +1 через обратный обход: ", values);
    std::sort(values.begin(), values.end()); // Прямые указатели поддерживают random access.
    printVector("После std::sort: ", values);
    std::reverse(values.rbegin(), values.rend()); // Обратному итератору достаточно bidirectional.
    printVector("После std::reverse через reverse_iterator: ", values);
    std::cout << "std::distance(rbegin, rend) = " << std::distance(view.rbegin(), view.rend()) << '\n';

    Vector<std::string> words(2, "слово");
    std::cout << "operator-> у обратного итератора, empty() строки: " << words.rbegin()->empty() << '\n';
    Vector<int> empty;
    Vector<int> one(1, 9);
    std::cout << "Пустой диапазон: begin == end: " << (empty.begin() == empty.end())
              << ", rbegin == rend: " << (empty.rbegin() == empty.rend()) << '\n';
    auto single = one.rbegin();
    std::cout << "Один элемент: " << *single;
    ++single;
    std::cout << ", следующий итератор равен rend: " << (single == one.rend()) << '\n';
  }
}

int main()
{
  std::cout << std::boolalpha;
  try
  {
    demonstrateConstructionAndGrowth();
    demonstrateModificationAndLifetime();
    demonstrateRuleOfFive();
    demonstrateArithmeticAndErrors();
    demonstrateIterators();
    runAmortizedAnalysis(std::cout);
  }
  catch (const std::exception& error)
  {
    std::cerr << "Ошибка выполнения: " << error.what() << '\n';
    return 1;
  }
}
