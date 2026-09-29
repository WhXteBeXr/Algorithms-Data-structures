#include <iostream>
#include <algorithm>
#include <numeric>
#include <iomanip>
#include "Vector.h"

void demoRuleOfFive() {
    std::cout << "=== Демонстрация правила пяти ===" << std::endl;

    Vector<int> a;
    for (int i = 1; i <= 5; ++i) a.push_back(i * 10);

    std::cout << "a = ";
    for (int x : a) std::cout << x << " ";
    std::cout << std::endl;

    // Конструктор копирования
    Vector<int> b(a);
    b[0] = 999;
    std::cout << "b (копия a, изменён первый элемент): ";
    for (int x : b) std::cout << x << " ";
    std::cout << "\na осталась без изменений: ";
    for (int x : a) std::cout << x << " ";
    std::cout << std::endl;

    // Оператор присваивания копированием + защита от самоприсваивания
    Vector<int> c;
    c = a;
    c = c; // самоприсваивание не должно ничего сломать
    std::cout << "c = a (через operator=): ";
    for (int x : c) std::cout << x << " ";
    std::cout << std::endl;

    // Конструктор перемещения
    Vector<int> d(std::move(c));
    std::cout << "d (перемещён из c): ";
    for (int x : d) std::cout << x << " ";
    std::cout << "\nc после перемещения, size = " << c.size() << std::endl;

    // Оператор присваивания перемещением
    Vector<int> e;
    e = std::move(d);
    std::cout << "e (перемещён из d): ";
    for (int x : e) std::cout << x << " ";
    std::cout << "\nd после перемещения, size = " << d.size() << std::endl;
}

void demoIteratorCategories() {
    std::cout << "\n=== Демонстрация категорий итератора ===" << std::endl;

    Vector<int> v;
    for (int i = 1; i <= 8; ++i) v.push_back(i);

    // --- Input/Forward: однократный и многократный проход ---
    std::cout << "Range-based for (использует begin()/end()): ";
    for (int x : v) std::cout << x << " ";
    std::cout << std::endl;

    auto it1 = v.begin();
    std::cout << "Многократный проход (forward-свойство): первый проход - ";
    for (auto it = it1; it != v.end(); ++it) std::cout << *it << " ";
    std::cout << "\n    второй проход с той же начальной точки it1 - ";
    for (auto it = it1; it != v.end(); ++it) std::cout << *it << " ";
    std::cout << std::endl;

    // --- Bidirectional: движение назад ---
    std::cout << "Обратный обход через оператор --: ";
    auto it = v.end();
    --it;
    for (;; --it) {
        std::cout << *it << " ";
        if (it == v.begin()) break;
    }
    std::cout << std::endl;

    // --- Random access: произвольный доступ, +, -, [] ---
    auto rit = v.begin();
    std::cout << "Random access: *(begin()+3) = " << *(rit + 3) << std::endl;
    std::cout << "Random access: begin()[5] = " << rit[5] << std::endl;
    std::cout << "Random access: end() - begin() = " << (v.end() - v.begin()) << std::endl;

    // --- Совместимость со стандартными алгоритмами STL ---
    std::cout << "\nstd::distance(begin, end) = "
              << std::distance(v.begin(), v.end()) << std::endl;

    auto found = std::find(v.begin(), v.end(), 5);
    std::cout << "std::find(5): найден на позиции "
              << std::distance(v.begin(), found) << std::endl;

    int total = std::accumulate(v.begin(), v.end(), 0);
    std::cout << "std::accumulate: сумма элементов = " << total << std::endl;

    // Перемешаем и отсортируем, чтобы показать, что нужен именно
    // random access iterator (у forward/bidirectional такого нет)
    Vector<int> unsorted;
    for (int x : {5, 3, 8, 1, 9, 2}) unsorted.push_back(x);

    std::cout << "\nДо сортировки: ";
    for (int x : unsorted) std::cout << x << " ";

    std::sort(unsorted.begin(), unsorted.end()); // требует random access iterator
    std::cout << "\nПосле std::sort: ";
    for (int x : unsorted) std::cout << x << " ";
    std::cout << std::endl;
}

void demoBasicOperations() {
    std::cout << "\n=== Базовые операции ===" << std::endl;

    Vector<int> v;
    std::cout << "size=" << v.size() << " capacity=" << v.capacity() << std::endl;

    for (int i = 1; i <= 10; ++i) {
        v.push_back(i);
        std::cout << "push_back(" << i << "): size=" << v.size()
                   << " capacity=" << v.capacity() << std::endl;
    }

    v.pop_back();
    std::cout << "После pop_back: size=" << v.size() << std::endl;

    v.resize(15, -1);
    std::cout << "После resize(15, -1): ";
    for (int x : v) std::cout << x << " ";
    std::cout << std::endl;

    v.clear();
    std::cout << "После clear(): size=" << v.size()
              << " (capacity сохранилась: " << v.capacity() << ")" << std::endl;

    std::cout << "\n--- Граничные случаи ---" << std::endl;
    Vector<int> empty;
    try {
        empty.pop_back();
    } catch (const std::underflow_error& e) {
        std::cout << "Поймано исключение: " << e.what() << std::endl;
    }
    try {
        int x = empty[0];
        (void)x;
    } catch (const std::out_of_range& e) {
        std::cout << "Поймано исключение: " << e.what() << std::endl;
    }
}

int main() {
    demoBasicOperations();
    demoRuleOfFive();
    demoIteratorCategories();
    return 0;
}