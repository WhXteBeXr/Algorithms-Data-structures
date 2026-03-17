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

vector<int> userVectorInput()
{
  int vectorLength;
  vector<int> userVector;

  cout << "Введите длину массива: ";
  cin >> vectorLength;
  cout << endl;

  for (int i = 0; i < vectorLength; i++)
  {
    int enteredNumber;
    cout << "Введите число: ";
    cin >> enteredNumber;
    userVector.push_back(enteredNumber);
  }

  return userVector;
}

bool containsDigitThree(int number)
{
  number = abs(number);
  if (number < 0)
    return false;

  while (number > 0)
  {
    // Если остаток будет равен 3, иначе сдвинем число на одну позицию и отбросим остаток
    if (number % 10 == 3)
      return true;
    number /= 10;
  }

  return false;
}

void taskOneUserInput()
{
  int userInput;
  const vector<int> userVector = userVectorInput();
  vector<int> clearVector;

  for (const int& number : userVector)
  {
    if (!isFullSquare(number))
      clearVector.push_back(number);
  }

  // Вывод всего массивов
  cout << endl << "Исходный массив: ";
  for (const int& vectorNumber : userVector)
    cout << vectorNumber << " ";

  cout << endl << "Массив убранных полных квадратов: ";
  for (const int& clearNumber : clearVector)
    cout << clearNumber << " ";
}


void taskTwoTriplets()
{
  const vector<int> userVector = userVectorInput();
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
