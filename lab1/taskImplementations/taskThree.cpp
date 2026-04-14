#include <algorithm>
#include <fstream>
#include <iostream>

#include "../lab1Functions.h"

/* Организовать текстовый файл. Заменить в файле все маленькие латинские буквы на большие.
 * (Создавая новый дополнительный файл) */

using namespace std;

void taskThree()
{
  ifstream lowercase("../lab1/testFiles/tThree/lowercase.txt");
  ofstream uppercase("../lab1/testFiles/tThree/uppercase.txt");

  if (!lowercase.is_open() || !uppercase.is_open())
  {
    cerr << "Ошибка открытия одного из файлов" << endl;
    exit(1);
  }

  string currentString;
  while (lowercase >> currentString)
  {
    // Проходим с помощью итераторов от начала до конца строки и применяем toupper,
    // получая результат обратно в currentString
    ranges::transform(currentString, currentString.begin(), ::toupper);
    uppercase << currentString << " ";
  }

  cout << "Символы записаны в файл uppercase.txt" << endl;
  lowercase.close();
  uppercase.close();
}
