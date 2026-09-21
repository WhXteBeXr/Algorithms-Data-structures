#include <fstream>
#include <iostream>

#include "../labFunctions.h"

using namespace std;

/* Организовать файл целых чисел. В новый файл записать элементы файла занимающие нечётные позиции, в другой новый файл
 * элементы файла занимающие чётные позиции. */

void taskFour()
{
  ifstream input("../lab1/testFiles/tFour/input.txt");
  ofstream even("../lab1/testFiles/tFour/even.txt");
  ofstream odd("../lab1/testFiles/tFour/odd.txt");

  if (!input.is_open() || !even.is_open() || !odd.is_open())
  {
    cerr << "Ошибка открытия одного из файлов" << endl;
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

  cout << "Числа записаны в файлы even.txt и odd.txt" << endl;
  input.close();
  even.close();
  odd.close();
}
