#include <fstream>
#include <iostream>

#include "../labFunctions.h"

using namespace std;

// Дано 2 файла вещественных чисел а1.txt и а2.txt. Найти сумму положительных элементов в двух файлов.

void taskTwo()
{
  // Открываем для чтения по пути от корня проекта
  ifstream fileA("../lab1/testFiles/tTwo/fileA.txt");
  ifstream fileB("../lab1/testFiles/tTwo/fileB.txt");

  // Проверяем открыты ли файлы
  if (!fileA.is_open() || !fileB.is_open())
  {
    cerr << "Ошибка открытия одного из файлов" << endl;
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
