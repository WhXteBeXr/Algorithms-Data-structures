#include "Benchmark.h"
#include "DoublyLinkedList.h"
#include "SinglyLinkedList.h"

#include <algorithm>
#include <exception>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <memory>
#include <string>
#include <utility>

namespace
{
  template <typename List>
  void printList(const char* label, const List& values)
  {
    std::cout << label;
    values.print(std::cout);
    std::cout << ", размер = " << values.size() << '\n';
  }

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

  void demonstrateSingly()
  {
    std::cout << "=== Односвязный список ===\n";
    SinglyLinkedList<int> values;
    printList("Пустой список: ", values);
    std::cout << "empty = " << values.empty() << ", find(99) == end: " << (values.find(99) == values.end()) << '\n';

    // Сначала создаём границы цепочки, затем вставляем между существующими узлами.
    values.push_back(10);
    values.push_back(20);
    values.push_back(30);
    values.push_front(5);
    values.insert_after(values.find(20), 25);
    values.insert_before(values.find(10), 7);
    values.insert_after(values.find(30), 35); // Меняется tail.
    values.insert_before(values.end(), 40);
    printList("После вставок: ", values);
    std::cout << "Максимум = " << values.findMax() << '\n';

    // Удаление из середины, головы и хвоста должно сохранять всю оставшуюся цепочку.
    values.remove(7);
    values.erase(values.find(5));
    values.remove(40);
    values.push_back(50); // Используем обновлённый хвост после удаления.
    values.pop_front();
    values.pop_back();
    printList("После удалений: ", values);
    std::cout << "remove(999) = " << values.remove(999) << ", erase(end) = " << values.erase(values.end()) << '\n';

    values[1] = 111;
    const auto& view = values;
    std::cout << "const [1] = " << view[1] << ", переходов = " << view.getLastAccessSteps() << '\n';
    // Разворачиваем связи, не копируя значения. Проверяем новый tail добавлением.
    values.reverse();
    values.push_back(1);
    printList("После reverse и push_back(1): ", values);
    for (int& value : values)
    {
      ++value;
    }
    printList("Изменение через range-based for: ", values);
    std::cout << "const-обход: ";
    std::copy(view.begin(), view.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << "\nstd::distance = " << std::distance(view.begin(), view.end()) << '\n';

    auto it = values.begin();
    auto previous = it++;
    SinglyLinkedList<int>::ConstIterator readOnly = it;
    std::cout << "Постфиксный ++: было " << *previous << ", стало " << *it
              << ", const-итератор совпадает: " << (readOnly == it) << '\n';
    const auto found = view.find(111 + 1);
    std::cout << "const find(112) найден: " << (found != view.end()) << '\n';

    // Пограничный случай одного узла: после удаления обе границы должны обнулиться.
    values.clear();
    values.reverse();
    values.push_front(9);
    values.pop_back();
    values.push_back(8);
    values.pop_front();
    std::cout << "После удаления единственных узлов: пуст = " << values.empty() << '\n';
    values.push_back(42);
    printList("Повторное заполнение: ", values);
  }

  void printBoth(const char* label, const DoublyLinkedList<int>& values)
  {
    printList(label, values);
    std::cout << "В обратном порядке: ";
    values.printReverse(std::cout);
    std::cout << '\n';
  }

  void demonstrateDoubly()
  {
    std::cout << "\n=== Двусвязный список ===\n";
    DoublyLinkedList<int> values;
    values.push_back(10);
    values.push_back(20);
    values.push_back(30);
    values.push_front(5);
    values.insert_after(values.find(20), 25);
    values.insert_before(values.find(10), 7);
    values.insert_before(values.begin(), 1);
    values.insert_after(values.find(30), 35);
    values.insert_before(values.end(), 40);
    printBoth("После вставок: ", values);

    values.erase(values.find(20));
    values.erase(values.begin());
    values.remove(40);
    values.pop_front();
    values.pop_back();
    printBoth("После удалений: ", values);
    std::cout << "remove(999) = " << values.remove(999) << ", erase(end) = " << values.erase(values.end()) << '\n';
    values[1] = 99;
    const auto& view = values;
    const auto last = static_cast<std::ptrdiff_t>(values.size()) - 1;
    std::cout << "const [last] = " << view[last] << ", переходов = " << view.getLastAccessSteps() << '\n';

    // Сохранённый end не содержит старую копию tail, поэтому видит новый хвост.
    auto savedEnd = values.end();
    values.push_back(50);
    auto tail = savedEnd;
    --tail;
    std::cout << "--сохранённый end после push_back(50): " << *tail << '\n';
    values.pop_back();
    tail = savedEnd;
    std::cout << "--сохранённый end после pop_back: " << *--tail << '\n';

    // Изменение связей сохраняет узлы, вывод в обе стороны показывает их согласованность.
    values.reverse();
    values.push_front(8);
    values.push_back(9);
    printBoth("После reverse и вставок по краям: ", values);
    std::cout << "Обход через --end(): ";
    for (auto it = view.end(); it != view.begin();)
    {
      std::cout << *--it << ' ';
    }
    std::cout << '\n';
    auto it = values.end();
    auto oldEnd = it--;
    DoublyLinkedList<int>::ConstIterator readOnly = it;
    std::cout << "Постфиксный --: прежний == end: " << (oldEnd == values.end())
              << ", текущий = " << *readOnly << '\n';
    std::reverse(values.begin(), values.end()); // Алгоритм меняет значения, а не связи узлов.
    printBoth("После std::reverse: ", values);
    std::cout << "std::distance = " << std::distance(values.begin(), values.end()) << '\n';
  }

  void demonstrateDuplicatesAndTypes()
  {
    std::cout << "\n=== Дубликаты и разные типы ===\n";
    DoublyLinkedList<int> repeated;
    for (int value : {1, 2, 2, 3, 1, 4, 3, 3, 5})
    {
      repeated.push_back(value);
    }
    printBoth("До удаления дубликатов: ", repeated);
    repeated.removeDuplicates();
    printBoth("После удаления дубликатов: ", repeated);
    repeated.removeDuplicates(); // Повторный вызов уже не меняет список.
    repeated.clear();
    repeated.removeDuplicates();
    repeated.reverse();
    repeated.push_back(7);
    repeated.push_back(7);
    repeated.push_back(7);
    repeated.removeDuplicates();
    repeated.pop_back();
    std::cout << "После одинаковых значений и удаления единственного: пуст = " << repeated.empty() << '\n';

    SinglyLinkedList<std::string> words;
    words.push_back("alpha");
    words.push_back("beta");
    std::cout << "Строки, максимум = " << words.findMax() << ", длина первой через -> = " << words.begin()->size() << '\n';
    DoublyLinkedList<double> fractions;
    fractions.push_front(1.5);
    fractions.push_back(2.75);
    printList("Дробные значения: ", fractions);

    // Умный указатель в узле позволяет увидеть освобождение ресурса без
    // диагностических деструкторов и без отдельных тестовых программ.
    DoublyLinkedList<std::shared_ptr<int>> owners;
    auto resource = std::make_shared<int>(7);
    std::weak_ptr<int> observer = resource;
    owners.push_back(resource);
    resource.reset();
    owners.clear();
    std::cout << "Ресурс узла освобождён при clear: " << observer.expired() << '\n';
  }

  template <typename List>
  void demonstrateOwnership(const char* name)
  {
    std::cout << "\n=== Копирование и перемещение: " << name << " ===\n";
    List original;
    for (int value : {1, 2, 3})
    {
      original.push_back(value);
    }
    List copy(original);
    copy[0] = 99;
    printList("Исходный: ", original);
    printList("Изменённая копия: ", copy);
    List assigned;
    assigned.push_back(8);
    assigned = original; // Здесь вызывается присваивание, а не конструктор копии.
    assigned.pop_front();
    printList("После присваивания и удаления у копии: ", assigned);
    List& same = original;
    original = same;
    List moved(std::move(original));
    original.push_back(77);
    printList("Перемещённый: ", moved);
    printList("Повторно заполненный источник: ", original);
    List target;
    target.push_back(-1);
    target = std::move(assigned);
    List& targetAlias = target;
    target = std::move(targetAlias);
    printList("Перемещающее присваивание и самоперемещение: ", target);
    std::cout << "Источник присваивания пуст: " << assigned.empty() << '\n';
    target.swap(copy);
    printList("Первый список после swap: ", target);
    printList("Второй список после swap: ", copy);
  }

  template <typename List>
  void demonstrateErrors(const char* name)
  {
    std::cout << "\n=== Граничные случаи: " << name << " ===\n";
    List values;
    showError("pop_front пустого", [&] { values.pop_front(); });
    showError("pop_back пустого", [&] { values.pop_back(); });
    showError("Индекс пустого", [&] { values[0]; });
    showError("Разыменование end", [&] { *values.end(); });
    showError("++end", [&] { ++values.end(); });
    values.push_back(1);
    showError("Отрицательный индекс", [&] { values[-1]; });
    showError("Индекс size()", [&] { values[static_cast<std::ptrdiff_t>(values.size())]; });
    showError("Вставка после end", [&] { values.insert_after(values.end(), 5); });
    List foreign;
    foreign.push_back(2);
    showError("Вставка по чужому итератору", [&] { values.insert_before(foreign.begin(), 3); });
    showError("Удаление по чужому итератору", [&] { values.erase(foreign.begin()); });
    printList("После ошибок: ", values);
  }

  template <typename List>
  void printAccess(const char* kind, int count)
  {
    List values;
    for (int i = 0; i < count; ++i)
    {
      values.push_back(i);
    }
    // После каждого [] сразу читаем счётчик, пока его не заменил следующий доступ.
    for (int index : {0, count / 2, count - 1})
    {
      const List& view = values;
      const int result = view[index];
      std::cout << std::setw(8) << count << std::setw(10) << kind << std::setw(10) << index
                << std::setw(12) << result << std::setw(12) << view.getLastAccessSteps() << '\n';
    }
  }

  void demonstrateIndexComplexity()
  {
    std::cout << "\n=== Число переходов при доступе по индексу ===\n"
              << std::setw(8) << "N" << std::setw(10) << "List" << std::setw(10) << "index"
              << std::setw(12) << "value" << std::setw(12) << "link steps" << '\n';
    for (int count : {10, 100, 1000})
    {
      printAccess<SinglyLinkedList<int>>("Singly", count);
      printAccess<DoublyLinkedList<int>>("Doubly", count);
    }
  }
}

int main()
{
  std::cout << std::boolalpha;
  try
  {
    demonstrateSingly();
    demonstrateDoubly();
    demonstrateDuplicatesAndTypes();
    demonstrateOwnership<SinglyLinkedList<int>>("Singly");
    demonstrateOwnership<DoublyLinkedList<int>>("Doubly");
    demonstrateErrors<SinglyLinkedList<int>>("Singly");
    demonstrateErrors<DoublyLinkedList<int>>("Doubly");
    SinglyLinkedList<int> emptySingly;
    showError("Максимум пустого списка", [&] { emptySingly.findMax(); });
    DoublyLinkedList<int> emptyDoubly;
    showError("--end пустого двусвязного", [&] { --emptyDoubly.end(); });
    emptyDoubly.push_back(1);
    showError("--begin двусвязного", [&] { --emptyDoubly.begin(); });
    demonstrateIndexComplexity();
    runListComparison(std::cout);
  }
  catch (const std::exception& error)
  {
    std::cerr << "Ошибка выполнения: " << error.what() << '\n';
    return 1;
  }
}
