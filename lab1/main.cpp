#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

void taskOne()
{
  // Открываем для чтения по пути от корня проекта
  ifstream file("../lab1/testFiles/tOne/taskOneFile.txt");

  // Проверяем открыт ли файл
  if (!file.is_open())
  {
    cout << "Ошибка открытия файла" << endl;
    exit(1);
  }

  double currentNum;
  unsigned int positiveCounter = 0;
  unsigned int negativeCounter = 0;

  cout << "Числа из файла:" << endl;
  while (file >> currentNum) // Проходимся по каждому из чисел
  {
    cout << currentNum << " ";
    if (currentNum > 0)
      positiveCounter++;
    else if (currentNum < 0)
      negativeCounter++;
  }
  file.close(); // Не забыть закрыть файл

  cout << endl << "\nПоложительных чисел: " << positiveCounter << "\nОтрицательных чисел: " << negativeCounter << endl;
}

void taskTwo()
{
  ifstream fileA("../lab1/testFiles/tTwo/fileA.txt");
  ifstream fileB("../lab1/testFiles/tTwo/fileB.txt");

  if (!fileA.is_open() || !fileB.is_open())
  {
    cout << "Ошибка открытия одного из файлов" << endl;
    exit(1);
  }

  double currentNum;
  double filesSum = 0;

  while (fileA >> currentNum)
    filesSum += currentNum;

  cout << "Сумма файла A: " << filesSum << endl;
  while (fileB >> currentNum)
    filesSum += currentNum;

  fileA.close();
  fileB.close();

  cout << "Сумма файлов A & B: " << filesSum << endl;
}

void taskThree()
{
  ifstream lowercase("../lab1/testFiles/tThree/lowercase.txt");
  ofstream uppercase("../lab1/testFiles/tThree/uppercase.txt");

  if (!lowercase.is_open() || !uppercase.is_open())
  {
    cout << "Ошибка открытия одного из файлов" << endl;
    exit(1);
  }

  string currentString;
  while (lowercase >> currentString)
  {
    // Проходим с помощью итераторов от начала до конца строки и применяем toupper,
    // получая результат обратно в currentString
    ranges::transform(currentString, currentString.begin(), ::toupper);
    uppercase << currentString << " ";
  }

  cout << "Символы записаны в файл" << endl;
  lowercase.close();
  uppercase.close();
}

void taskFour()
{
  ifstream input("../lab1/testFiles/tFour/input.txt");
  ofstream even("../lab1/testFiles/tFour/even.txt");
  ofstream odd("../lab1/testFiles/tFour/odd.txt");

  if (!input.is_open() || !even.is_open() || !odd.is_open())
  {
    cout << "Ошибка открытия одного из файлов" << endl;
    exit(1);
  }

  int currentNum;
  unsigned int currentPosition = 1;

  while (input >> currentNum)
  {
    if (currentPosition % 2 == 0)
      even << currentNum << " ";
    else
      odd << currentNum << " ";
    currentPosition++;
  }

  cout << "Числа записаны в файлы" << endl;
  input.close();
  even.close();
  odd.close();
}

int main()
{
  unsigned int userChoice;
  cout << "Выберите задание: "
          "\n1 - Положительные и отрицательные"
          "\n2 - Сумма чисел файлов"
          "\n3 - Строчные в прописные"
          "\n4 - Позиции чисел"
          "\n5 -"
       << endl;
  cin >> userChoice;
  cout << endl;

  switch (userChoice)
  {
  case 1:
    taskOne();
    break;
  case 2:
    taskTwo();
    break;
  case 3:
    taskThree();
    break;
  case 4:
    taskFour();
    break;
  case 5:
    break;
  case 6:
    break;
  case 7:
    break;
  case 8:
    break;
  case 9:
    break;
  case 10:
    break;

  default:
    cout << "Выбранного задания нет в списке";
  }
}
