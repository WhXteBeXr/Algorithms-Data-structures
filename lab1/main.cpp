#include <iostream>

#include "./lab1Functions.h"

using namespace std;


int main()
{
  unsigned int userChoice;
  cout << "Выберите задание: "
          "\n1 - Положительные и отрицательные"
          "\n2 - Сумма чисел файлов"
          "\n3 - Строчные в прописные"
          "\n4 - Позиции чисел"
          "\n5 - Самый молодой"
          "\n6 - Произведение чисел"
          "\n7 - Матрицы"
          "\n8 - Бинарные студенты"
          "\n9 - Двойные двоичные числа"
          "\n10 - Стипендии"
       << endl;
  cin >> userChoice;
  cout << endl;

  switch (userChoice)
  {
  case 1:
    vectorClassTask();
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
    taskFive();
    break;
  case 6:
    taskSix();
    break;
  case 7:
    taskSeven();
    break;
  case 8:
    taskEight();
    break;
  case 9:
    taskNine();
    break;
  case 10:
    taskTen();
    break;

  default:
    cout << "Выбранного задания нет в списке" << endl;
  }
  return 0;
}
