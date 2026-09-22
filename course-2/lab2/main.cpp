#include <iostream>
#include <vector>
#include <string>
#include "Vector.h"
#include "Benchmark.h"

void demoBasicOperations() {
    std::cout << "=== Демонстрация основных операций ===" << std::endl;

    Vector<int> v(2);
    std::cout << "Создан Vector<int>, ёмкость: " << v.capacity()
              << ", размер: " << v.size() << std::endl;

    for (int i = 1; i <= 6; ++i) {
        v.push_back(i * 10);
        std::cout << "push_back(" << i * 10 << ") -> размер: " << v.size()
                   << ", ёмкость: " << v.capacity() << std::endl;
    }

    std::cout << "\nСодержимое через operator[]: ";
    for (std::size_t i = 0; i < v.size(); ++i) {
        std::cout << v[i] << " ";
    }
    std::cout << std::endl;

    v.pop_back();
    std::cout << "После pop_back(): размер " << v.size()
              << ", последний элемент " << v[v.size() - 1] << std::endl;

    std::cout << "\n--- reserve() ---" << std::endl;
    std::cout << "Ёмкость до reserve(100): " << v.capacity() << std::endl;
    v.reserve(100);
    std::cout << "Ёмкость после reserve(100): " << v.capacity()
              << ", размер не изменился: " << v.size() << std::endl;

    std::cout << "\n--- Работа с другим типом (string) ---" << std::endl;
    Vector<std::string> words(2);
    words.push_back("Привет");
    words.push_back("мир");
    for (std::size_t i = 0; i < words.size(); ++i) {
        std::cout << words[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "\n--- Граничные случаи ---" << std::endl;
    Vector<int> empty(1);
    try {
        empty.pop_back();
    } catch (const std::out_of_range& e) {
        std::cout << "Поймано исключение: " << e.what() << std::endl;
    }
    try {
        int x = empty[0];
        (void)x;
    } catch (const std::out_of_range& e) {
        std::cout << "Поймано исключение: " << e.what() << std::endl;
    }
}

void demoComplexity() {
    std::cout << "\n=== Сложность операций Vector<T> ===" << std::endl;
    std::cout << "push_back:  O(1) амортизированно, O(n) в момент расширения" << std::endl;
    std::cout << "pop_back:   O(1)" << std::endl;
    std::cout << "operator[]: O(1)" << std::endl;
    std::cout << "size:       O(1)" << std::endl;
    std::cout << "capacity:   O(1)" << std::endl;
    std::cout << "reserve:    O(n) при увеличении ёмкости, иначе O(1)" << std::endl;
}

void compareWithSTL() {
    std::cout << "\n=== Сравнение с std::vector (push_back без reserve) ===" << std::endl;
    std::cout << "N\t\tVector<T>, мкс\tstd::vector, мкс" << std::endl;

    std::vector<int> sizes = {10000, 100000, 500000, 1000000};

    for (int n : sizes) {
        long long myTime = measureTime([&]() {
            Vector<int> v(1);
            for (int i = 0; i < n; ++i) v.push_back(i);
        });

        long long stlTime = measureTime([&]() {
            std::vector<int> v;
            for (int i = 0; i < n; ++i) v.push_back(i);
        });

        std::cout << n << "\t\t" << myTime << "\t\t" << stlTime << std::endl;
    }

    std::cout << "\n=== То же самое, но с предварительным reserve(n) ===" << std::endl;
    std::cout << "N\t\tVector<T>, мкс\tstd::vector, мкс" << std::endl;

    for (int n : sizes) {
        long long myTime = measureTime([&]() {
            Vector<int> v(1);
            v.reserve(n);
            for (int i = 0; i < n; ++i) v.push_back(i);
        });

        long long stlTime = measureTime([&]() {
            std::vector<int> v;
            v.reserve(n);
            for (int i = 0; i < n; ++i) v.push_back(i);
        });

        std::cout << n << "\t\t" << myTime << "\t\t" << stlTime << std::endl;
    }
}

int main() {
    demoBasicOperations();
    demoComplexity();
    compareWithSTL();
    return 0;
}