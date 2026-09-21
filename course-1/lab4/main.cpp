#include <iostream>
#include <stdexcept>

namespace
{
  class IntVector
  {
private:
    int* data;
    int size;
    int capacity;

public:
    using iterator = int*;
    using const_iterator = const int*;

    // Конструкторы
    IntVector() : size(0), capacity(10) { data = new int[capacity]; }

    IntVector(const int n) : size(n), capacity(n)
    {
      if (n < 0)
        throw std::invalid_argument("Размер не может быть отрицательным");
      if (n == 0)
        capacity = 1;
      data = new int[capacity];
      for (int i = 0; i < size; ++i)
        data[i] = 0;
    }

    // Копирование (deep copy)
    IntVector(const IntVector& other) : size(other.size), capacity(other.capacity)
    {
      data = new int[capacity];
      for (int i = 0; i < size; ++i)
        data[i] = other.data[i];
    }

    // Перемещение
    IntVector(IntVector&& other) noexcept : data(other.data), size(other.size), capacity(other.capacity)
    {
      other.data = nullptr;
      other.size = 0;
      other.capacity = 0;
    }

    // Деструктор
    ~IntVector() { delete[] data; }

    // Операторы присваивания
    IntVector& operator=(const IntVector& other)
    {
      if (this != &other)
      {
        int* newData = new int[other.capacity];
        for (int i = 0; i < other.size; ++i)
          newData[i] = other.data[i];
        delete[] data;
        data = newData;
        size = other.size;
        capacity = other.capacity;
      }
      return *this;
    }

    IntVector& operator=(IntVector&& other) noexcept
    {
      if (this != &other)
      {
        delete[] data;
        data = other.data;
        size = other.size;
        capacity = other.capacity;
        other.data = nullptr;
        other.size = 0;
        other.capacity = 0;
      }
      return *this;
    }

    // Доступ по индексу
    int& operator[](const int index)
    {
      if (index < 0 || index >= size)
        throw std::out_of_range("Индекс вне диапазона");
      return data[index];
    }
    const int& operator[](const int index) const
    {
      if (index < 0 || index >= size)
        throw std::out_of_range("Индекс вне диапазона");
      return data[index];
    }

    // Методы доступа
    int getSize() const { return size; }
    int getCapacity() const { return capacity; }
    bool isEmpty() const { return size == 0; }

    // Изменение
    void pushBack(const int value)
    {
      if (size >= capacity)
      {
        const int newCapacity = (capacity == 0) ? 1 : capacity * 2;
        reserve(newCapacity);
      }
      data[size] = value;
      size++;
    }

    void popBack()
    {
      if (isEmpty())
        throw std::underflow_error("Вектор пуст");
      size--;
    }

    void reserve(const int newCapacity)
    {
      if (newCapacity <= capacity)
        return;
      int* newData = new int[newCapacity];
      for (int i = 0; i < size; ++i)
        newData[i] = data[i];
      delete[] data;
      data = newData;
      capacity = newCapacity;
    }

    void shrinkToFit()
    {
      if (capacity == size)
        return;
      const int newCapacity = (size == 0) ? 1 : size;
      int* newData = new int[newCapacity];
      for (int i = 0; i < size; ++i)
        newData[i] = data[i];
      delete[] data;
      data = newData;
      capacity = newCapacity;
    }

    void insert(const iterator pos, const int value)
    {
      const int index = static_cast<int>(pos - data);
      if (index < 0 || index > size)
        throw std::out_of_range("Некорректная позиция вставки");
      if (size >= capacity)
        reserve((capacity == 0) ? 1 : capacity * 2);
      for (int i = size; i > index; --i)
        data[i] = data[i - 1];
      data[index] = value;
      size++;
    }

    // Итераторы
    iterator begin() { return data; }
    iterator end() { return data + size; }
    const_iterator begin() const { return data; }
    const_iterator end() const { return data + size; }

    // Вывод
    void print() const
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
  };
} // namespace

int main()
{
  IntVector v1;
  for (int i = 1; i <= 5; ++i)
    v1.pushBack(i);
  v1.print();

  v1.insert(v1.begin() + 2, 100); // вставка в середину через итератор
  v1.print();

  v1.popBack();
  v1.print();

  v1.reserve(50);
  std::cout << "Capacity после reserve: " << v1.getCapacity() << "\n";

  v1.shrinkToFit();
  std::cout << "Capacity после shrinkToFit: " << v1.getCapacity() << "\n";

  // Проверка range-based for (работает благодаря begin/end)
  std::cout << "Range-based for: ";
  for (const int x : v1)
    std::cout << x << " ";
  std::cout << "\n";

  const IntVector v2 = std::move(v1); // move-конструктор
  std::cout << "v2 после move: ";
  v2.print();
  std::cout << "v1.getSize() после move: " << v1.getSize() << "\n"; // 0

  return 0;
}
