#pragma once
#include <stdexcept>
#include <cstddef>

// Собственная упрощённая реализация динамического массива,
// аналог std::vector с ручным управлением памятью.
// Реализация методов дана прямо в заголовке, т.к. шаблонный код
// должен быть виден компилятору в каждой единице трансляции,
// где он используется.
template <typename T>
class Vector {
private:
    T* data;
    std::size_t count;      // фактическое количество элементов
    std::size_t cap;        // выделенная ёмкость

    // O(n) - выделение новой памяти и копирование всех элементов
    void reallocate(std::size_t newCapacity) {
        T* newData = new T[newCapacity];
        for (std::size_t i = 0; i < count; ++i) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        cap = newCapacity;
    }

public:
    explicit Vector(std::size_t initialCapacity = 4)
        : data(nullptr), count(0), cap(initialCapacity) {
        if (cap == 0) cap = 1;
        data = new T[cap];
    }

    ~Vector() {
        delete[] data;
    }

    // Запрещаем копирование, чтобы не думать о глубоком копировании
    // в рамках этой лабораторной работы
    Vector(const Vector&) = delete;
    Vector& operator=(const Vector&) = delete;

    // O(1) амортизированно. Худший случай O(n) - когда происходит
    // удвоение ёмкости, но такое случается редко (раз в log n вызовов),
    // поэтому в среднем на один вызов приходится O(1)
    void push_back(const T& value) {
        if (count == cap) {
            reallocate(cap * 2);
        }
        data[count++] = value;
    }

    // O(1) - память не освобождается, просто уменьшается count
    void pop_back() {
        if (count == 0) {
            throw std::out_of_range("Vector::pop_back: вектор пуст");
        }
        --count;
    }

    // O(1) - прямой доступ по индексу через арифметику указателей
    T& operator[](std::size_t index) {
        if (index >= count) {
            throw std::out_of_range("Vector::operator[]: индекс вне диапазона");
        }
        return data[index];
    }

    const T& operator[](std::size_t index) const {
        if (index >= count) {
            throw std::out_of_range("Vector::operator[]: индекс вне диапазона");
        }
        return data[index];
    }

    // O(1)
    std::size_t size() const {
        return count;
    }

    // O(1)
    std::size_t capacity() const {
        return cap;
    }

    // O(1)
    bool empty() const {
        return count == 0;
    }

    // O(n) - если newCapacity > текущей ёмкости, иначе O(1) (ничего не делает)
    void reserve(std::size_t newCapacity) {
        if (newCapacity > cap) {
            reallocate(newCapacity);
        }
    }
};