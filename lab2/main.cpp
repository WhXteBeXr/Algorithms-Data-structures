#include <cmath>
#include <iostream>
#include <numeric>
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
  cout << endl << "Введите длину массива: ";
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

bool isPrime(const int& number)
{
  if (number < 2)
    return false;

  // Проверка на простое число перебором делителей
  int divider = 3;
  while (divider <= round(sqrt(number)))
  {
    if (number % divider == 0)
      return false;
    divider++;
  }
  return true;
}

void taskOneUserInput()
{
  const vector<int> userVector = userVectorInput();
  vector<int> clearVector;

  for (const int& number : userVector)
  {
    if (!isFullSquare(number))
      clearVector.push_back(number);
  }

  // Вывод всех массивов
  cout << endl << "Исходный массив: ";
  for (const int& vectorNumber : userVector)
    cout << vectorNumber << " ";

  cout << endl << "Массив убранных полных квадратов: ";
  for (const int& clearNumber : clearVector)
    cout << clearNumber << " ";

  cout << endl << "Сумма массива убранных квадратов: " << accumulate(clearVector.begin(), clearVector.end(), 0) << endl;
}


void taskTwoTriplets()
{
  unsigned int tripletsCount = 0;
  int minSum = numeric_limits<int>::max();
  const vector<int> userVector = userVectorInput();

  cout << endl;
  for (size_t i = 0; i < userVector.size() - 2; i++)
  {
    cout << "Рассмотриваемая тройка: " << userVector[i] << " " << userVector[i + 1] << " " << userVector[i + 2] << endl;
    if (containsDigitThree(userVector[i]) and containsDigitThree(userVector[i + 1]) and
        containsDigitThree(userVector[i + 2]))
    {
      // Тестовый набор 3 13 31 23 1 / 23 13 31 3 51
      if (int tripletsSum = userVector[i] + userVector[i + 1] + userVector[i + 2]; isPrime(tripletsSum))
      {
        tripletsCount++;
        if (tripletsSum < minSum)
          minSum = tripletsSum;
      }
    }
  }

  cout << endl
       << "Количество троек, в каждом из чисел которых есть цифра 3: " << tripletsCount << endl
       << "Минимальная сумма троек: " << minSum << endl;
}

int main()
{
  unsigned int userTaskChoose;
  cout << "Выберите задание: "
          "\n1 - (Массив без полных квадратов)"
          "\n2 - (Тройки чисел)"
       << endl;
  cin >> userTaskChoose;

  switch (userTaskChoose)
  {
  case 1:
    taskOneUserInput();
    break;
  case 2:
    taskTwoTriplets();
    break;
  default:
    cout << "Выбранного задания нет в списке";
  }
  return 0;
}
