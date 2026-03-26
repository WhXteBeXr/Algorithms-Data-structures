#include <fstream>
#include <iostream>
#include <string>

using namespace std;

void taskOne()
{
  ifstream file("../lab1/taskOneFile.txt");
  if (!file.is_open())
  {
    cout << "Ошибка открытия файла" << endl;
    exit(1);
  }

  double currentNum;
  unsigned int positiveCounter = 0;
  unsigned int negativeCounter = 0;

  cout << endl << "Числа из файла:" << endl;
  while (file >> currentNum)
  {
    cout << currentNum << " ";
    if (currentNum > 0)
      positiveCounter++;
    else if (currentNum < 0)
      negativeCounter++;
  }
  file.close();

  cout << endl << "\nПоложительных чисел: " << positiveCounter << "\nОтрицательных чисел: " << negativeCounter << endl;
}

int main()
{
  unsigned int userChoice;
  cout << "Выберите задание: "
          "\n1 - (Положительные и отрицательные)" << endl;
  cin >> userChoice;

  switch (userChoice)
  {
  case 1:
    taskOne();
    break;
  case 2:
    break;
  case 3:
    break;
  default:
    cout << "Выбранного задания нет в списке";
  }
}
