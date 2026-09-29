#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <numeric>
#include <string>
#include "SinglyLinkedList.h"
#include "DoublyLinkedList.h"
#include "Benchmark.h"

void demoSingly() {
    std::cout << "=== Односвязный список ===" << std::endl;
    SinglyLinkedList<int> list;

    list.push_back(10);
    list.push_back(20);
    list.push_back(30);
    list.push_front(5);
    std::cout << "После push_back x3 и push_front: ";
    list.print();
    std::cout << "front = " << list.front() << ", back = " << list.back()
              << ", size = " << list.size() << std::endl;

    list.pop_front();
    list.pop_back();
    std::cout << "После pop_front и pop_back: ";
    list.print();

    auto it = list.find(10);
    list.insert_after(it, 15);
    std::cout << "После insert_after(10, 15): ";
    list.print();

    list.erase_after(list.find(10));
    std::cout << "После erase_after(10): ";
    list.print();

    list.push_back(40);
    list.push_back(50);
    std::cout << "getAt(2) = " << list.getAt(2) << std::endl;

    list.remove(40);
    std::cout << "После remove(40): ";
    list.print();

    list.reverse();
    std::cout << "После reverse: ";
    list.print();
    std::cout << "back после reverse = " << list.back() << " (tail обновлён)" << std::endl;
}

void demoDoubly() {
    std::cout << "\n=== Двусвязный список ===" << std::endl;
    DoublyLinkedList<int> list;

    for (int i = 1; i <= 5; ++i) list.push_back(i * 10);
    list.push_front(5);
    std::cout << "Прямой порядок:   ";
    list.print();
    std::cout << "Обратный порядок: ";
    list.printReverse();

    list.pop_front();
    list.pop_back();
    std::cout << "После pop_front и pop_back: ";
    list.print();

    auto it = list.find(30);
    list.insert(it, 25);
    list.insert_after(list.find(30), 35);
    std::cout << "После insert(перед 30, 25) и insert_after(30, 35): ";
    list.print();

    list.erase(list.find(25));
    std::cout << "После erase(25): ";
    list.print();

    std::cout << "getAt(3) = " << list.getAt(3) << " (обход с ближайшего конца)" << std::endl;

    list.reverse();
    std::cout << "После reverse: ";
    list.print();
    std::cout << "Обратный порядок после reverse: ";
    list.printReverse();
}

void demoIterators() {
    std::cout << "\n=== Итераторы ===" << std::endl;

    SinglyLinkedList<int> s;
    DoublyLinkedList<int> d;
    for (int i = 1; i <= 6; ++i) {
        s.push_back(i);
        d.push_back(i);
    }

    std::cout << "range-based for (односвязный): ";
    for (int x : s) std::cout << x << " ";
    std::cout << std::endl;

    // Изменение элементов через обычный итератор
    for (auto it = s.begin(); it != s.end(); ++it) *it *= 10;
    std::cout << "После *it *= 10: ";
    s.print();

    // const_iterator не даёт изменять элементы
    const SinglyLinkedList<int>& cs = s;
    std::cout << "const_iterator: ";
    for (auto it = cs.cbegin(); it != cs.cend(); ++it) std::cout << *it << " ";
    std::cout << std::endl;

    // Двусвязный: обход назад через --end()
    std::cout << "Обратный обход двусвязного через --: ";
    auto it = d.end();
    while (it != d.begin()) {
        --it;
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // Совместимость со STL-алгоритмами
    std::cout << "std::distance (односвязный) = " << std::distance(s.begin(), s.end()) << std::endl;
    std::cout << "std::accumulate (односвязный) = " << std::accumulate(s.begin(), s.end(), 0) << std::endl;

    auto found = std::find(d.begin(), d.end(), 4);
    std::cout << "std::find(4) в двусвязном: позиция " << std::distance(d.begin(), found) << std::endl;

    // std::reverse требует bidirectional-итератор - односвязный список не подошёл бы
    std::reverse(d.begin(), d.end());
    std::cout << "std::reverse (двусвязный): ";
    d.print();
}

void demoEdgeCases() {
    std::cout << "\n=== Граничные случаи и память ===" << std::endl;

    SinglyLinkedList<int> s;
    DoublyLinkedList<int> d;

    try { s.pop_front(); } catch (const std::underflow_error& e) { std::cout << e.what() << std::endl; }
    try { s.pop_back(); }  catch (const std::underflow_error& e) { std::cout << e.what() << std::endl; }
    try { d.pop_front(); } catch (const std::underflow_error& e) { std::cout << e.what() << std::endl; }
    try { d.pop_back(); }  catch (const std::underflow_error& e) { std::cout << e.what() << std::endl; }
    try { s.front(); }     catch (const std::underflow_error& e) { std::cout << e.what() << std::endl; }
    try { d.getAt(0); }    catch (const std::out_of_range& e)    { std::cout << e.what() << std::endl; }
    try { s.erase_after(s.begin()); } catch (const std::out_of_range& e) { std::cout << e.what() << std::endl; }

    std::cout << "remove из пустого списка: " << (s.remove(1) ? "удалён" : "не найден") << std::endl;
    std::cout << "find в пустом списке == end(): " << (s.find(1) == s.end() ? "да" : "нет") << std::endl;

    // Последний элемент: head и tail должны сброситься
    s.push_back(1);
    s.pop_back();
    s.push_back(2);
    std::cout << "После удаления единственного элемента и новой вставки: ";
    s.print();

    // Правило пяти
    SinglyLinkedList<std::string> a;
    a.push_back("один");
    a.push_back("два");

    SinglyLinkedList<std::string> b(a);
    b.front() = "изменено";
    std::cout << "Копия изменена, оригинал: ";
    a.print();

    b = b; // самоприсваивание
    std::cout << "После b = b: ";
    b.print();

    SinglyLinkedList<std::string> c(std::move(a));
    std::cout << "После перемещения a -> c: c = ";
    c.print();
    std::cout << "размер a = " << a.size() << std::endl;

    DoublyLinkedList<int> dd;
    dd.push_back(1);
    dd.push_back(2);
    DoublyLinkedList<int> dd2;
    dd2 = dd;
    dd2.push_back(3);
    std::cout << "Двусвязный: копия ";
    dd2.print();
    std::cout << "            оригинал ";
    dd.print();
}

template <typename List>
long long timePushBack(int n) {
    return measureTime([&]() {
        List l;
        for (int i = 0; i < n; ++i) l.push_back(i);
    });
}

template <typename List>
long long timePushFront(int n) {
    return measureTime([&]() {
        List l;
        for (int i = 0; i < n; ++i) l.push_front(i);
    });
}

template <typename List>
long long timePopBack(int n) {
    List l;
    for (int i = 0; i < n; ++i) l.push_back(i);
    return measureTime([&]() {
        for (int i = 0; i < n; ++i) l.pop_back();
    });
}

template <typename List>
long long timeFindLast(int n) {
    List l;
    for (int i = 0; i < n; ++i) l.push_back(i);
    volatile int sink = 0; // чтобы компилятор не выбросил поиск как неиспользуемый
    return measureTime([&]() {
        sink = *l.find(n - 1);
    });
}

void printRow(int n, long long a, long long b) {
    std::cout << std::setw(10) << n << std::setw(18) << a << std::setw(18) << b << std::endl;
}

void printHeader(const std::string& title) {
    std::cout << "\n" << title << std::endl;
    std::cout << std::setw(10) << "N" << std::setw(18) << "Односвязный, мкс"
              << std::setw(18) << "Двусвязный, мкс" << std::endl;
}

void comparePerformance() {
    std::cout << "\n=== Сравнение производительности ===" << std::endl;

    printHeader("push_back (оба O(1))");
    for (int n : {10000, 100000, 1000000}) {
        printRow(n, timePushBack<SinglyLinkedList<int>>(n), timePushBack<DoublyLinkedList<int>>(n));
    }

    printHeader("push_front (оба O(1))");
    for (int n : {10000, 100000, 1000000}) {
        printRow(n, timePushFront<SinglyLinkedList<int>>(n), timePushFront<DoublyLinkedList<int>>(n));
    }

    // Для односвязного каждый pop_back - O(n), поэтому размеры меньше
    printHeader("pop_back n раз (односвязный O(n) на вызов, двусвязный O(1))");
    for (int n : {5000, 10000, 20000}) {
        printRow(n, timePopBack<SinglyLinkedList<int>>(n), timePopBack<DoublyLinkedList<int>>(n));
    }

    printHeader("find последнего элемента (оба O(n))");
    for (int n : {10000, 100000, 1000000}) {
        printRow(n, timeFindLast<SinglyLinkedList<int>>(n), timeFindLast<DoublyLinkedList<int>>(n));
    }
}

int main() {
    demoSingly();
    demoDoubly();
    demoIterators();
    demoEdgeCases();
    comparePerformance();
    return 0;
}