#include "SparseMatrix.h"
#include <cmath>

void SparseMatrix::checkIndex(const int r, const int c) const
{
  if (r < 0 || r >= rowCount || c < 0 || c >= colCount)
  {
    throw std::out_of_range("Индекс вне диапазона матрицы");
  }
}

void SparseMatrix::checkSameShape(const SparseMatrix& o) const
{
  if (rowCount != o.rowCount || colCount != o.colCount)
  {
    throw std::invalid_argument("Размеры матриц не совпадают");
  }
}

void SparseMatrix::checkSquare() const
{
  if (rowCount != colCount)
  {
    throw std::invalid_argument("Матрица должна быть квадратной");
  }
}

// Линейный поиск элемента (r, c) среди хранимых
int SparseMatrix::find(const int r, const int c) const
{
  for (int i = 0; i < size; ++i)
  {
    if (rows[i] == r && cols[i] == c)
      return i;
  }
  return -1;
}

// Новые массивы нужной длины, перенос данных, освобождение старых
void SparseMatrix::reallocate(const int newSize)
{
  auto* newValues = new double[newSize];
  int* newRows = new int[newSize];
  int* newCols = new int[newSize];
  const int count = size < newSize ? size : newSize;
  for (int i = 0; i < count; ++i)
  {
    newValues[i] = values[i];
    newRows[i] = rows[i];
    newCols[i] = cols[i];
  }
  delete[] values;
  delete[] rows;
  delete[] cols;
  values = newValues;
  rows = newRows;
  cols = newCols;
  size = newSize;
}

// Каждый наш хранимый элемент должен совпасть с элементом матрицы o
bool SparseMatrix::elementsEqualIn(const SparseMatrix& o) const
{
  for (int i = 0; i < size; ++i)
  {
    if (values[i] != o.get(rows[i], cols[i]))
      return false;
  }
  return true;
}

SparseMatrix::SparseMatrix() : SparseMatrix(0, 0) {}

SparseMatrix::SparseMatrix(int r, int c) :
    values(nullptr), rows(nullptr), cols(nullptr), size(0), rowCount(r), colCount(c)
{
  if (r < 0 || c < 0)
  {
    throw std::invalid_argument("Размеры матрицы не могут быть отрицательными");
  }
}

// Глубокое копирование: свои массивы + копия элементов
SparseMatrix::SparseMatrix(const SparseMatrix& other) :
    values(new double[other.size]), rows(new int[other.size]), cols(new int[other.size]), size(other.size),
    rowCount(other.rowCount), colCount(other.colCount)
{
  for (int i = 0; i < size; ++i)
  {
    values[i] = other.values[i];
    rows[i] = other.rows[i];
    cols[i] = other.cols[i];
  }
}

SparseMatrix::~SparseMatrix()
{
  delete[] values;
  delete[] rows;
  delete[] cols;
}

SparseMatrix& SparseMatrix::operator=(const SparseMatrix& other)
{
  if (this != &other)
  { // защита от m = m
    auto* newValues = new double[other.size];
    int* newRows = new int[other.size];
    int* newCols = new int[other.size];
    for (int i = 0; i < other.size; ++i)
    {
      newValues[i] = other.values[i];
      newRows[i] = other.rows[i];
      newCols[i] = other.cols[i];
    }
    delete[] values;
    delete[] rows;
    delete[] cols;
    values = newValues;
    rows = newRows;
    cols = newCols;
    size = other.size;
    rowCount = other.rowCount;
    colCount = other.colCount;
  }
  return *this;
}

int SparseMatrix::getRows() const { return rowCount; }
int SparseMatrix::getCols() const { return colCount; }
int SparseMatrix::getSize() const { return size; }

// Элемент (r, c): если не хранится — это ноль
double SparseMatrix::get(const int r, const int c) const
{
  checkIndex(r, c);
  int pos = find(r, c);
  return pos >= 0 ? values[pos] : 0.0;
}

// Запись элемента. Нули не хранятся: запись нуля удаляет элемент
void SparseMatrix::set(const int r, const int c, const double value)
{
  checkIndex(r, c);
  int pos = find(r, c);
  if (pos >= 0 && value != 0)
  { // элемент есть — меняем значение
    values[pos] = value;
  }
  else if (pos >= 0)
  { // элемент есть, новое значение 0 — удаляем
    values[pos] = values[size - 1]; // последний элемент ставим на место удаляемого
    rows[pos] = rows[size - 1];
    cols[pos] = cols[size - 1];
    reallocate(size - 1); // «отрезаем» последнюю позицию
  }
  else if (value != 0)
  { // элемента нет — добавляем в конец
    reallocate(size + 1);
    values[size - 1] = value;
    rows[size - 1] = r;
    cols[size - 1] = c;
  }
}

double SparseMatrix::norm() const
{
  double sum = 0;
  for (int i = 0; i < size; ++i)
  {
    sum += values[i] * values[i];
  }
  return std::sqrt(sum);
}

// Сложение: копия левой матрицы + добавляем ненулевые элементы правой
SparseMatrix SparseMatrix::operator+(const SparseMatrix& o) const
{
  checkSameShape(o);
  SparseMatrix result(*this);
  for (int i = 0; i < o.size; ++i)
  {
    result.set(o.rows[i], o.cols[i], result.get(o.rows[i], o.cols[i]) + o.values[i]);
  }
  return result;
}

SparseMatrix SparseMatrix::operator-(const SparseMatrix& o) const
{
  checkSameShape(o);
  SparseMatrix result(*this);
  for (int i = 0; i < o.size; ++i)
  {
    result.set(o.rows[i], o.cols[i], result.get(o.rows[i], o.cols[i]) - o.values[i]);
  }
  return result;
}

// Оптимизированное умножение матриц: перебираем только пары ненулевых элементов,
// у которых столбец левого равен строке правого. Нулевые элементы не трогаем
SparseMatrix SparseMatrix::operator*(const SparseMatrix& o) const
{
  if (colCount != o.rowCount)
  {
    throw std::invalid_argument("Число столбцов левой матрицы должно равняться числу строк правой");
  }
  SparseMatrix result(rowCount, o.colCount);
  for (int i = 0; i < size; ++i)
  {
    for (int j = 0; j < o.size; ++j)
    {
      if (cols[i] == o.rows[j])
      {
        int r = rows[i];
        int c = o.cols[j];
        result.set(r, c, result.get(r, c) + values[i] * o.values[j]);
      }
    }
  }
  return result;
}

// Деление матрицы на число
SparseMatrix SparseMatrix::operator/(double k) const
{
  if (k == 0)
  {
    throw std::invalid_argument("Деление на ноль");
  }
  SparseMatrix result(rowCount, colCount);
  for (int i = 0; i < size; ++i)
  {
    result.set(rows[i], cols[i], values[i] / k);
  }
  return result;
}

SparseMatrix& SparseMatrix::operator+=(const SparseMatrix& o)
{
  *this = *this + o;
  return *this;
}

SparseMatrix& SparseMatrix::operator-=(const SparseMatrix& o)
{
  *this = *this - o;
  return *this;
}

SparseMatrix& SparseMatrix::operator*=(const double k)
{
  *this = *this * k;
  return *this;
}

SparseMatrix& SparseMatrix::operator/=(const double k)
{
  *this = *this / k;
  return *this;
}

// Матрицы равны, если совпадают размеры и все элементы (проверяем в обе стороны)
bool SparseMatrix::operator==(const SparseMatrix& o) const
{
  if (rowCount != o.rowCount || colCount != o.colCount)
    return false;
  return elementsEqualIn(o) && o.elementsEqualIn(*this);
}

bool SparseMatrix::operator!=(const SparseMatrix& o) const { return !(*this == o); }

// Порядок «больше/меньше» задаём по норме матрицы
bool SparseMatrix::operator<(const SparseMatrix& o) const { return norm() < o.norm(); }
bool SparseMatrix::operator>(const SparseMatrix& o) const { return o < *this; }
bool SparseMatrix::operator<=(const SparseMatrix& o) const { return !(o < *this); }
bool SparseMatrix::operator>=(const SparseMatrix& o) const { return !(*this < o); }

// ++m: прибавляет единичную матрицу (единицы на главной диагонали)
SparseMatrix& SparseMatrix::operator++()
{
  checkSquare();
  for (int i = 0; i < rowCount; ++i)
  {
    set(i, i, get(i, i) + 1);
  }
  return *this;
}

// m++: возвращает старое значение, а сама матрица изменяется
SparseMatrix SparseMatrix::operator++(int)
{
  SparseMatrix old(*this);
  ++(*this);
  return old;
}

SparseMatrix& SparseMatrix::operator--()
{
  checkSquare();
  for (int i = 0; i < rowCount; ++i)
  {
    set(i, i, get(i, i) - 1);
  }
  return *this;
}

SparseMatrix SparseMatrix::operator--(int)
{
  SparseMatrix old(*this);
  --(*this);
  return old;
}

double SparseMatrix::operator()(const int r, const int c) const { return get(r, c); }

// m[k] — k-й хранимый элемент (порядок хранения не гарантируется).
// Через неконстантную версию нельзя записывать 0: нули не должны храниться
double& SparseMatrix::operator[](const int k)
{
  if (k < 0 || k >= size)
  {
    throw std::out_of_range("Индекс вне диапазона");
  }
  return values[k];
}

const double& SparseMatrix::operator[](const int k) const
{
  if (k < 0 || k >= size)
  {
    throw std::out_of_range("Индекс вне диапазона");
  }
  return values[k];
}

SparseMatrix::operator double() const { return norm(); }
SparseMatrix::operator bool() const { return size > 0; }

// Матрица * число: умножаем только хранимые элементы
SparseMatrix operator*(const SparseMatrix& m, const double k)
{
  SparseMatrix result(m.rowCount, m.colCount);
  for (int i = 0; i < m.size; ++i)
  {
    result.set(m.rows[i], m.cols[i], m.values[i] * k); // при k = 0 элемент не добавится
  }
  return result;
}

// Число * матрица — то же самое (симметричность)
SparseMatrix operator*(const double k, const SparseMatrix& m) { return m * k; }

// Вывод в виде обычной (плотной) таблицы
std::ostream& operator<<(std::ostream& os, const SparseMatrix& m)
{
  for (int i = 0; i < m.rowCount; ++i)
  {
    for (int j = 0; j < m.colCount; ++j)
    {
      os << m.get(i, j) << "\t";
    }
    os << "\n";
  }
  return os;
}

// Ввод: «строки столбцы k», затем k троек «строка столбец значение»
std::istream& operator>>(std::istream& is, SparseMatrix& m)
{
  int r = 0, c = 0, k = 0;
  is >> r >> c >> k;
  SparseMatrix temp(r, c); // неверные размеры -> исключение
  for (int i = 0; i < k; ++i)
  {
    int row = 0, col = 0;
    double value = 0;
    is >> row >> col >> value;
    temp.set(row, col, value); // неверный индекс -> исключение
  }
  m = temp;
  return is;
}
