#include <iostream>
#include <vector>

#include "IntVector.h"

using namespace std;

IntVector::IntVector() : size(0), vectorElement(0)
{
  this->numbers = {};
  cout << "Конструктор по умолчанию" << endl;
}

IntVector::IntVector(const size_t size, const int element) : size(size), vectorElement(element)
{
  this->numbers = vector<int>(size, element);
  this->size = size;
  cout << "Был вызван параметризированныйы констурктор" << endl;
}

IntVector::IntVector(IntVector&& other)
{
  this->size = other.size;
  this->vectorElement = other.vectorElement;
  this->numbers = other.numbers;

  other.size = 0;
  other.vectorElement = 0;
  other.numbers.clear();
  cout << "Вызван перемещающий коснтруктор" << endl;
}

IntVector& IntVector::operator=(const IntVector& other)
{
  if (this != &other)
  {
    this->size = other.size;
    this->vectorElement = other.vectorElement;
    this->numbers = other.numbers;
  }
  cout << "Вызван копирующий конструктор" << endl;

  return *this;
}

IntVector& IntVector::operator=(IntVector&& other)
{
  if (this != &other)
  {
    this->size = other.size;
    this->vectorElement = other.vectorElement;
    this->numbers = other.numbers;

    other.size = 0;
    other.vectorElement = 0;
    other.numbers.clear();
  }
  cout << "Вызван конструктор перемещающего присваивания" << endl;
  return *this;
}

IntVector::~IntVector()
{
  cout << "Вызван деконструктор для массива размером: " << size << " и исходными элементами: " << vectorElement << endl;
}

void IntVector::setVectorElement(const int element)
{
  this->vectorElement = element;
  this->numbers = vector<int>(this->size, this->vectorElement);
}

void IntVector::setSize(const size_t size)
{
  this->size = size;
  cout << "Был установлен размер: " << size << endl;

  this->numbers = vector<int>(this->size, this->vectorElement);
  cout << "Вектор был заполнен до указанной длины" << endl;
}

size_t IntVector::getSize() const { return numbers.size(); }

void IntVector::vectorReserve(const int length)
{
  if (length <= 0)
    cout << "Указано неподходящее значение резервируемой длины" << endl;
  this->numbers.reserve(length);
}

void IntVector::vectorShrink() { this->numbers.shrink_to_fit(); }

void IntVector::pushBack(const int element)
{
  numbers.push_back(element);
  cout << "Элементе добавлен в конец" << endl;
}

void IntVector::popBack()
{
  const int lastNumber = numbers.back();
  numbers.pop_back();
  cout << "Был удален последний элемент: " << lastNumber << endl;
}

void IntVector::print() const
{
  cout << "Вектор одержит элементы: ";
  for (const int number : numbers)
  {
    cout << number << " ";
  }
  cout << endl;
}
