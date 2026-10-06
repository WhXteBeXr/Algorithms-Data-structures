#include "IntVector.h"

// Выделяет новую память, переносит элементы, освобождает старую.
// Сначала выделяем новую память: если new бросит исключение, старые данные целы.
void IntVector::reallocate(const int newCapacity)
{
  int* newData = new int[newCapacity];
  for (int i = 0; i < size; ++i)
  {
    newData[i] = data[i];
  }
  delete[] data;
  data = newData;
  capacity = newCapacity;
}

IntVector::IntVector() : data(nullptr), size(0), capacity(10) { data = new int[capacity]; }

IntVector::IntVector(const int n) : data(nullptr), size(n), capacity(n > 0 ? n : 1)
{
  if (n < 0)
  {
    throw std::invalid_argument("Размер не может быть отрицательным");
  }
  data = new int[capacity];
  for (int i = 0; i < size; ++i)
  {
    data[i] = 0;
  }
}

IntVector::IntVector(const int n, const int value) : IntVector(n)
{
  // Делегируем создание конструктору размера, затем заполняем значением
  for (int i = 0; i < size; ++i)
  {
    data[i] = value;
  }
}

// Копирования: новая память + копия элементов (глубокое копирование)
IntVector::IntVector(const IntVector& other) : data(nullptr), size(other.size), capacity(other.capacity)
{
  data = new int[capacity];
  for (int i = 0; i < size; ++i)
  {
    data[i] = other.data[i];
  }
}

// Перемещения: забираем память у other, ничего не копируем
IntVector::IntVector(IntVector&& other) noexcept : data(other.data), size(other.size), capacity(other.capacity)
{
  other.data = nullptr; // other больше не владеет памятью
  other.size = 0;
  other.capacity = 0;
}

IntVector::~IntVector()
{
  delete[] data; // delete[] от nullptr безопасен
}

IntVector& IntVector::operator=(const IntVector& other)
{
  if (this != &other)
  { // защита от v = v
    int* newData = new int[other.capacity]; // сначала выделяем новую память
    for (int i = 0; i < other.size; ++i)
    {
      newData[i] = other.data[i];
    }
    delete[] data; // потом освобождаем старую
    data = newData;
    size = other.size;
    capacity = other.capacity;
  }
  return *this;
}

IntVector& IntVector::operator=(IntVector&& other) noexcept
{
  if (this != &other)
  {
    delete[] data; // освобождаем свою память
    data = other.data; // забираем чужую
    size = other.size;
    capacity = other.capacity;
    other.data = nullptr; // обнуляем источник
    other.size = 0;
    other.capacity = 0;
  }
  return *this;
}

int& IntVector::operator[](const int index)
{
  if (index < 0 || index >= size)
  {
    throw std::out_of_range("Индекс вне диапазона");
  }
  return data[index];
}

const int& IntVector::operator[](const int index) const
{
  if (index < 0 || index >= size)
  {
    throw std::out_of_range("Индекс вне диапазона");
  }
  return data[index];
}

// Конкатенация: копия левого вектора + элементы правого
IntVector IntVector::operator+(const IntVector& other) const
{
  IntVector result(*this); // копия левого операнда
  for (int i = 0; i < other.size; ++i)
  {
    result.pushBack(other.data[i]); // дописываем элементы правого
  }
  return result; // исходные векторы не изменились
}

// Умножение на число: каждый элемент копии умножаем на k
IntVector IntVector::operator*(int k) const
{
  IntVector result(*this);
  for (int i = 0; i < result.size; ++i)
  {
    result.data[i] *= k;
  }
  return result;
}

// k * v — то же самое, что v * k
IntVector operator*(int k, const IntVector& v) { return v * k; }

int IntVector::getSize() const { return size; }
int IntVector::getCapacity() const { return capacity; }
bool IntVector::isEmpty() const { return size == 0; }

void IntVector::pushBack(const int value)
{
  if (size >= capacity)
  {
    // Удваиваем вместимость (для «пустого» вектора после перемещения — берём 1)
    reallocate(capacity == 0 ? 1 : capacity * 2);
  }
  data[size] = value;
  size++;
}

void IntVector::popBack()
{
  if (isEmpty())
  {
    throw std::underflow_error("Вектор пуст");
  }
  size--;
}

// Вставка по индексу: сдвигаем элементы вправо, ставим значение
void IntVector::insert(const int index, const int value)
{
  if (index < 0 || index > size)
  {
    throw std::out_of_range("Индекс вне диапазона");
  }
  if (size >= capacity)
  {
    reallocate(capacity == 0 ? 1 : capacity * 2);
  }
  for (int i = size; i > index; --i)
  {
    data[i] = data[i - 1];
  }
  data[index] = value;
  size++;
}

// Удаление по индексу: сдвигаем элементы влево
void IntVector::remove(const int index)
{
  if (index < 0 || index >= size)
  {
    throw std::out_of_range("Индекс вне диапазона");
  }
  for (int i = index; i < size - 1; ++i)
  {
    data[i] = data[i + 1];
  }
  size--;
}

void IntVector::clear()
{
  size = 0; // память не освобождаем, просто «забываем» элементы
}

// Изменение размера: новые элементы заполняются нулями
void IntVector::resize(const int newSize)
{
  if (newSize < 0)
  {
    throw std::invalid_argument("Размер не может быть отрицательным");
  }
  if (newSize > capacity)
  {
    reallocate(newSize);
  }
  for (int i = size; i < newSize; ++i)
  {
    data[i] = 0;
  }
  size = newSize;
}

// Возвращает индекс первого найденного элемента или -1
int IntVector::find(const int value) const
{
  for (int i = 0; i < size; ++i)
  {
    if (data[i] == value)
    {
      return i;
    }
  }
  return -1;
}

void IntVector::print() const
{
  std::cout << "[";
  for (int i = 0; i < size; ++i)
  {
    std::cout << data[i];
    if (i < size - 1)
      std::cout << ", ";
  }
  std::cout << "]\n";
}
