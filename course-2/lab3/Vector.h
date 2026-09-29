#pragma once
#include <stdexcept>
#include <iterator>
#include <algorithm>
#include <cstddef>

template <typename T>
class Vector {
private:
    T* data;
    int sz;
    int cap;

public:
    // ---------- Итератор с полной поддержкой категорий ----------
    // IsConst = true даёт const_iterator, false - обычный iterator.
    // Реализованы все операции, требуемые стандартом для
    // LegacyRandomAccessIterator - а это самая "сильная" категория,
    // она включает в себя все младшие:
    //   Input        - оператор*, ++, ==, != (однократный проход)
    //   Forward      - то же самое, но допускает многократный проход
    //                  (копию итератора можно разыменовывать снова)
    //   Bidirectional - добавляется оператор --
    //   Random access - добавляются +, -, +=, -=, [], <, >, <=, >=
    template <bool IsConst>
    class VectorIterator {
    public:
        // Типы, обязательные для std::iterator_traits -
        // именно по ним STL-алгоритмы узнают, что можно делать с итератором
        using iterator_category = std::random_access_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = typename std::conditional<IsConst, const T*, T*>::type;
        using reference          = typename std::conditional<IsConst, const T&, T&>::type;

    private:
        pointer ptr;

    public:
        explicit VectorIterator(pointer p = nullptr) : ptr(p) {}

        // --- Input / Forward: разыменование и переход к следующему ---
        reference operator*() const { return *ptr; }
        pointer operator->() const { return ptr; }

        VectorIterator& operator++() {          // префиксный ++
            ++ptr;
            return *this;
        }
        VectorIterator operator++(int) {        // постфиксный ++
            VectorIterator tmp = *this;
            ++ptr;
            return tmp;
        }

        // --- Bidirectional: переход назад ---
        VectorIterator& operator--() {
            --ptr;
            return *this;
        }
        VectorIterator operator--(int) {
            VectorIterator tmp = *this;
            --ptr;
            return tmp;
        }

        // --- Random access: сдвиг на произвольное число элементов ---
        VectorIterator& operator+=(difference_type n) { ptr += n; return *this; }
        VectorIterator& operator-=(difference_type n) { ptr -= n; return *this; }

        VectorIterator operator+(difference_type n) const { return VectorIterator(ptr + n); }
        VectorIterator operator-(difference_type n) const { return VectorIterator(ptr - n); }

        // Разница двух итераторов - нужна std::distance и сортировкам
        difference_type operator-(const VectorIterator& other) const { return ptr - other.ptr; }

        reference operator[](difference_type n) const { return *(ptr + n); }

        // Сравнения (нужны для random access и для循环 for(it != end()))
        bool operator==(const VectorIterator& other) const { return ptr == other.ptr; }
        bool operator!=(const VectorIterator& other) const { return ptr != other.ptr; }
        bool operator<(const VectorIterator& other) const { return ptr < other.ptr; }
        bool operator>(const VectorIterator& other) const { return ptr > other.ptr; }
        bool operator<=(const VectorIterator& other) const { return ptr <= other.ptr; }
        bool operator>=(const VectorIterator& other) const { return ptr >= other.ptr; }
    };

    using iterator = VectorIterator<false>;
    using const_iterator = VectorIterator<true>;

    // ---------- Конструкторы ----------

    Vector() : data(nullptr), sz(0), cap(0) {}

    explicit Vector(int n) : sz(n), cap(n) {
        data = new T[cap];
        for (int i = 0; i < sz; ++i) data[i] = T();
    }

    Vector(int n, const T& val) : sz(n), cap(n) {
        data = new T[cap];
        for (int i = 0; i < sz; ++i) data[i] = val;
    }

    // Конструктор копирования - глубокое копирование
    Vector(const Vector& other) : sz(other.sz), cap(other.cap) {
        data = new T[cap];
        for (int i = 0; i < sz; ++i) data[i] = other.data[i];
    }

    // Конструктор перемещения - "забирает" ресурсы у other,
    // не выполняя копирования элементов
    Vector(Vector&& other) noexcept
        : data(other.data), sz(other.sz), cap(other.cap) {
        other.data = nullptr;
        other.sz = 0;
        other.cap = 0;
    }

    // ---------- Деструктор ----------
    ~Vector() {
        delete[] data;
    }

    // ---------- Операторы присваивания ----------

    // copy-and-swap idiom: одновременно даёт защиту от самоприсваивания
    // и strong exception safety
    Vector& operator=(const Vector& other) {
        if (this != &other) {
            Vector temp(other);
            swap(temp);
        }
        return *this;
    }

    Vector& operator=(Vector&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            sz = other.sz;
            cap = other.cap;
            other.data = nullptr;
            other.sz = 0;
            other.cap = 0;
        }
        return *this;
    }

    // ---------- Доступ по индексу ----------
    T& operator[](int index) {
        if (index < 0 || index >= sz) {
            throw std::out_of_range("Vector::operator[]: индекс вне диапазона");
        }
        return data[index];
    }

    const T& operator[](int index) const {
        if (index < 0 || index >= sz) {
            throw std::out_of_range("Vector::operator[]: индекс вне диапазона");
        }
        return data[index];
    }

    // ---------- Методы доступа ----------
    int size() const { return sz; }
    int capacity() const { return cap; }
    bool empty() const { return sz == 0; }

    // ---------- Методы изменения ----------

    // O(n) при newCap > cap, иначе ничего не делает
    void reserve(int newCap) {
        if (newCap <= cap) return;
        T* newData = new T[newCap];
        for (int i = 0; i < sz; ++i) newData[i] = data[i];
        delete[] data;
        data = newData;
        cap = newCap;
    }

    // O(1) амортизированно (подробный анализ - в main.cpp/отчёте)
    void push_back(const T& value) {
        if (sz >= cap) {
            int newCap = (cap == 0) ? 1 : cap * 2;
            reserve(newCap);
        }
        data[sz++] = value;
    }

    // O(1)
    void pop_back() {
        if (empty()) {
            throw std::underflow_error("Vector::pop_back: вектор пуст");
        }
        --sz;
    }

    void resize(int newSize, const T& defaultValue = T()) {
        if (newSize < sz) {
            sz = newSize;
        } else if (newSize > sz) {
            if (newSize > cap) reserve(newSize);
            for (int i = sz; i < newSize; ++i) data[i] = defaultValue;
            sz = newSize;
        }
    }

    void clear() {
        sz = 0;
    }

    void swap(Vector& other) noexcept {
        std::swap(data, other.data);
        std::swap(sz, other.sz);
        std::swap(cap, other.cap);
    }

    // ---------- Итераторы ----------
    iterator begin() { return iterator(data); }
    iterator end() { return iterator(data + sz); }
    const_iterator begin() const { return const_iterator(data); }
    const_iterator end() const { return const_iterator(data + sz); }
    const_iterator cbegin() const { return const_iterator(data); }
    const_iterator cend() const { return const_iterator(data + sz); }
};