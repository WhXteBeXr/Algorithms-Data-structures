#include <iostream>

#include "labFunctions.h"

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
    taskFive();
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
    cout << "Выбранного задания нет в списке" << endl;
  }
  return 0;
}
