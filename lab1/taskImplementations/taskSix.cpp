#include <fstream>
#include <iostream>

#include "../lab1Functions.h"

using namespace std;

/* Создать текстовый файл в текстовом редакторе. Организовать просмотр содержимого файла и выполнения действий в
 * соответствии с условием индивидуального задания. Обеспечить сохранение всех полученных результатов в новый файл.
 *
 * Индивидуальное задание
 * Дан файл f, компоненты которого являются действительными числами. Найти произведение компонент файла.
 */

void taskSix()
{
  ifstream realNumbers("../lab1/testFiles/tSix/realNumbers.txt");
  ofstream output("../lab1/testFiles/tSix/output.txt");

  if (!realNumbers.is_open() || !output.is_open())
  {
    cerr << "Ошибка открытия одного из файлов" << endl;
    exit(1);
  }

  double currentNum;
  long double componentsProduct = 1;

  cout << "Числа записанные в файл:" << endl;
  while (realNumbers >> currentNum)
  {
    cout << currentNum << " ";
    componentsProduct *= currentNum;
  }

  output << "Произведение всех компонентов входного файла: " << componentsProduct << endl;

  realNumbers.close();
  output.close();

  cout << endl
       << "\nПроизведение всех компонентов входного файла: " << componentsProduct << endl
       << "Значение записано в output.txt" << endl;
}
