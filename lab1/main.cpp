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
  }

  double currentNum;
  double filesSum = 0;

  while (fileA >> currentNum)
    filesSum += currentNum;

  cout << "Сумма файла A: " << filesSum << endl;
  while (fileB >> currentNum)
    filesSum += currentNum;

  cout << "Сумма файлов A & B: " << filesSum << endl;
}

int main()
{
  unsigned int userChoice;
  cout << "Выберите задание: "
          "\n1 - Положительные и отрицательные"
          "\n2 - Сумма чисел файлов" << endl;
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
    break;
  default:
    cout << "Выбранного задания нет в списке";
  }
}
