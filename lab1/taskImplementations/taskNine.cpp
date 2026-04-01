#include <fstream>
#include <iostream>
#include <vector>

#include "../labFunctions.h"

using namespace std;

/* Представленные задания оформить в виде бинарного файла.
 *
 * Ввести в Memo или в StringGrid некоторое количество целых чисел и записать их в бинарный файл. Переписать бинарный
 * файл так, чтобы каждое значение, записанное в файле, было увеличено в два раза. Распечатать числа до и после
 * изменения данных в файле. */

void writeNumbers(const vector<int>& numbers, const string& filePath)
{
  ofstream numbersFile(filePath, ios::binary);
  if (!numbersFile.is_open())
  {
    cerr << "Ошибка открытия файла для записи" << endl;
    exit(1);
  }

  for (const int& number : numbers)
    numbersFile.write(reinterpret_cast<const char*>(&number), sizeof(number));
  numbersFile.close();
}

vector<int> readNumbers(const string& filePath)
{
  ifstream numbersFile(filePath, ios::binary);
  if (!numbersFile.is_open())
  {
    cerr << "Ошибка открытия файла для чтения" << endl;
    exit(1);
  }

  int number;
  vector<int> numbersVector;
  while (numbersFile.read(reinterpret_cast<char*>(&number), sizeof(number)))
    numbersVector.push_back(number);
  numbersFile.close();

  return numbersVector;
}

void printNumbers(const string& filePath)
{
  const vector<int> numbersVector = readNumbers(filePath);
  for (const int& number : numbersVector)
    cout << number << " ";
  cout << endl;
}

void doubleNumbers(const string& filePath)
{
  vector<int> numbersVector = readNumbers(filePath);
  for (int& number : numbersVector)
    number *= 2;

  writeNumbers(numbersVector, filePath);
}

void taskNine()
{
  const string filePath = "../lab1/testFiles/tNine/numbers.bin";
  const vector<int> originalNumbers = {5, -1, 2, 59, 102, -102, 89, 9, 1, 2, 333, 54, -98};

  cout << "Оригинальный массив чисел: ";
  for (const int& number : originalNumbers)
    cout << number << " ";
  cout << endl;
  cout << "Путь к файлу: " << filePath << endl;

  cout << endl << "--- Запись значений ---" << endl;
  writeNumbers(originalNumbers, filePath);

  cout << "Значения в бинарном файле: ";
  printNumbers(filePath);

  cout << endl << "--- Удваиваем значения чисел ---" << endl;
  doubleNumbers(filePath);

  cout << "Измененные значения из бинарного файла: ";
  printNumbers(filePath);
}
