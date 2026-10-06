#pragma once
#include <iostream>
#include <stdexcept>

class IntVector {
private:
  int* data;      // указатель на динамический массив
  int size;       // текущее количество элементов
  int capacity;   // вместимость (сколько элементов влезет без перевыделения)

  void reallocate(int newCapacity);   // вспомогательный: перевыделить память

public:
  // Конструкторы
  IntVector();                              // по умолчанию
  explicit IntVector(int n);                // размера n (нули)
  IntVector(int n, int value);              // размера n, заполненный value
  IntVector(const IntVector& other);        // копирования
  IntVector(IntVector&& other) noexcept;    // перемещения

  // Деструктор
  ~IntVector();

  // Операторы присваивания
  IntVector& operator=(const IntVector& other);       // копированием
  IntVector& operator=(IntVector&& other) noexcept;   // перемещением

  // Доступ по индексу
  int& operator[](int index);
  const int& operator[](int index) const;

  // Операторы варианта 3
  IntVector operator+(const IntVector& other) const;  // конкатенация
  IntVector operator*(int k) const;                   // умножение на число

  // Методы доступа
  int getSize() const;
  int getCapacity() const;
  bool isEmpty() const;

  // Методы изменения
  void pushBack(int value);
  void popBack();
  void insert(int index, int value);
  void remove(int index);
  void clear();
  void resize(int newSize);

  // Поиск и вывод
  int find(int value) const;
  void print() const;
};

// Умножение числа на вектор: k * v
IntVector operator*(int k, const IntVector& v);