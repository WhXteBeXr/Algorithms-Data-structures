#include <fstream>
#include <iostream>

#include "../lab1Functions.h"

using namespace std;

// Дан файл вещественных чисел a.txt Найти количество отрицательных и количество положительных элементов.

void taskOne()
{
  // Открываем для чтения по пути от корня проекта
  ifstream file("../lab1/testFiles/tOne/taskOneFile.txt");

  // Проверяем открыт ли файл
  if (!file.is_open())
  {
    cerr << "Ошибка открытия файла" << endl;
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