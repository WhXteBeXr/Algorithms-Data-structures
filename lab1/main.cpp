#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

bool isFullSquare(const int& number)
{
  if (number < 0)
    return false;

  // Если корень числа, возведенный в квадрат, будет равен исходному
  const double numSqrt = round(sqrt(number));
  if (pow(numSqrt, 2) == number)
    return true;

  return false;
}

void taskOneUserInput()
{
  unsigned int userInput;
  vector<int> userInputVector;
  vector<int> clearVector;

  cout << endl << "Введите длину вектора: ";
  cin >> userInput;
  cout << endl;

  // Наполнение массива числами пользователем
  for (unsigned int i = 0; i < userInput; i++)
  {
    int enteredNumber;
    cout << "Введите число: ";
    cin >> enteredNumber;

    // Добавление введенных чисел обычный массив
    userInputVector.push_back(enteredNumber);

    // Массив чисел не являющихся полными квадратами
    if (!isFullSquare(enteredNumber))
      clearVector.push_back(enteredNumber);
  }

  // Вывод всего массивов
  cout << endl << "Исходный массив: ";
  for (const int& vectorNumber : userInputVector)
    cout << vectorNumber << " ";

  cout << endl << "Массив убранных полных квадратов: ";
  for (const int& clearNumber : clearVector)
    cout << clearNumber << " ";
}

int main()
{
  unsigned int userTaskChoose;
  cout << "Выберите задание: \n1 - (Ввод массива)" << endl;
  cin >> userTaskChoose;

  switch (userTaskChoose)
  {
  case 1:
    taskOneUserInput();
    break;
  case 2:
    break;
  default:
    cout << "Выбранного задания нет в списке";
  }
  return 0;
}
