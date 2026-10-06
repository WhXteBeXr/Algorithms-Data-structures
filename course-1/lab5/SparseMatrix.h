#pragma once
#include <iostream>
#include <stdexcept>

// Разреженная матрица: хранятся только ненулевые элементы
// (формат «координатный список»: значение + номер строки + номер столбца)
class SparseMatrix
{
  private:
  double* values; // значения ненулевых элементов
  int* rows; // номера строк этих элементов
  int* cols; // номера столбцов этих элементов
  int size; // количество ненулевых элементов
  int rowCount; // число строк матрицы
  int colCount; // число столбцов матрицы

  // Вспомогательные методы
  void checkIndex(int r, int c) const; // индексы внутри матрицы?
  void checkSameShape(const SparseMatrix& o) const; // размеры совпадают?
  void checkSquare() const; // матрица квадратная?
  int find(int r, int c) const; // позиция элемента в массивах или -1
  void reallocate(int newSize); // изменить размер массивов
  bool elementsEqualIn(const SparseMatrix& o) const; // наши элементы совпадают с o?

  public:
  // Конструкторы и деструктор («правило трёх»)
  SparseMatrix(); // пустая матрица 0x0
  SparseMatrix(int r, int c); // матрица r x c из нулей
  SparseMatrix(const SparseMatrix& other); // копирования
  ~SparseMatrix();
  SparseMatrix& operator=(const SparseMatrix& other); // присваивания

  // Геттеры и сеттер
  int getRows() const;
  int getCols() const;
  int getSize() const; // число ненулевых элементов
  double get(int r, int c) const;
  void set(int r, int c, double value);
  double norm() const; // норма (корень из суммы квадратов)

  // Арифметика: матрица (+) матрица, матрица (-) матрица,
  // матрица (*) матрица, матрица (/) число
  SparseMatrix operator+(const SparseMatrix& o) const;
  SparseMatrix operator-(const SparseMatrix& o) const;
  SparseMatrix operator*(const SparseMatrix& o) const;
  SparseMatrix operator/(double k) const;

  // Составное присваивание
  SparseMatrix& operator+=(const SparseMatrix& o);
  SparseMatrix& operator-=(const SparseMatrix& o);
  SparseMatrix& operator*=(double k);
  SparseMatrix& operator/=(double k);

  // Сравнение: == и != по элементам, < > <= >= по норме
  bool operator==(const SparseMatrix& o) const;
  bool operator!=(const SparseMatrix& o) const;
  bool operator<(const SparseMatrix& o) const;
  bool operator>(const SparseMatrix& o) const;
  bool operator<=(const SparseMatrix& o) const;
  bool operator>=(const SparseMatrix& o) const;

  // ++ и -- : прибавить / вычесть единичную матрицу (только для квадратных)
  SparseMatrix& operator++(); // префиксный
  SparseMatrix operator++(int); // постфиксный
  SparseMatrix& operator--();
  SparseMatrix operator--(int);

  // Функтор: m(i, j) — элемент матрицы
  double operator()(int r, int c) const;

  // Индексация: m[k] — k-й хранимый (ненулевой) элемент
  double& operator[](int k);
  const double& operator[](int k) const;

  // Явные преобразования типов
  explicit operator double() const; // в число (норма)
  explicit operator bool() const; // true, если есть ненулевые элементы

  // Дружественные функции
  friend SparseMatrix operator*(const SparseMatrix& m, double k); // m * число
  friend SparseMatrix operator*(double k, const SparseMatrix& m); // число * m
  friend std::ostream& operator<<(std::ostream& os, const SparseMatrix& m);
  friend std::istream& operator>>(std::istream& is, SparseMatrix& m);
};
