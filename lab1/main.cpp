#include <algorithm>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

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

void taskTwo()
{
  ifstream fileA("../lab1/testFiles/tTwo/fileA.txt");
  ifstream fileB("../lab1/testFiles/tTwo/fileB.txt");

  if (!fileA.is_open() || !fileB.is_open())
  {
    cerr << "Ошибка открытия одного из файлов" << endl;
    exit(1);
  }

  double currentNum;
  double filesSum = 0;

  while (fileA >> currentNum)
    filesSum += currentNum;

  cout << "Сумма файла A: " << filesSum << endl;
  while (fileB >> currentNum)
    filesSum += currentNum;

  fileA.close();
  fileB.close();

  cout << "Сумма файлов A & B: " << filesSum << endl;
}

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

  cout << "Символы записаны в файл" << endl;
  lowercase.close();
  uppercase.close();
}

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

  cout << "Числа записаны в файлы" << endl;
  input.close();
  even.close();
  odd.close();
}

void taskFive()
{
  struct BirthDate
  {
    int year{}, month{}, day{};
  };

  struct Person
  {
    string surname, name, patronymic;
    string gender;
    string nationality;
    double height{}, weight{};
    BirthDate birthDate;
    string phoneNumber;
    string address;
  };

  ifstream personsInput("../lab1/testFiles/tFive/personInput.txt");

  if (!personsInput.is_open())
  {
    cout << "Ошибка открытия файла" << endl;
    exit(1);
  }

  vector<Person> allPersons;
  string currentLine;

  while (getline(personsInput, currentLine)) // Читаем целую строку
  {
    if (currentLine.empty()) // Идем дальше если пустая
      continue;

    Person currentPerson; // Создаем структуру
    istringstream iss(currentLine); // И создаем поток для чтения из строки
    if (!(iss >> currentPerson.surname >> currentPerson.name >> currentPerson.patronymic >> currentPerson.gender >>
          currentPerson.nationality >> currentPerson.height >> currentPerson.weight >> currentPerson.birthDate.year >>
          currentPerson.birthDate.month >> currentPerson.birthDate.day >> currentPerson.phoneNumber))
    {
      // Если в каком-то моменте не получится прочитать часть строки - будет ошибка
      cerr << "Ошибка чтения строки: " << currentLine << endl;
      continue;
    }

    // Читаем остаток строки и помещаем его в адрес
    getline(iss >> ws, currentPerson.address);
    allPersons.push_back(currentPerson);
  }
  personsInput.close();

  Person youngestPerson = allPersons[0]; // Создаем копию первого человека для дальнейшего сравнения

  // Ищем самого младшего и выводим список всех
  cout << "Информация всех людей из файла:" << endl;
  for (const Person& person : allPersons)
  {
    cout << person.surname << " " << person.name << " " << person.patronymic << " " << person.gender << " "
         << person.nationality << " Рост: " << person.height << " Вес: " << person.weight
         << " Дата рождения: " << person.birthDate.day << "/" << person.birthDate.month << "/" << person.birthDate.year
         << " Номер телефона: " << person.phoneNumber << " Адрес: " << person.address << endl;

    if (person.birthDate.year > youngestPerson.birthDate.year)
      youngestPerson = person;
    else if (person.birthDate.year == youngestPerson.birthDate.year)
    {
      if (person.birthDate.month > youngestPerson.birthDate.month)
      {
        youngestPerson = person;
      }
      else if (person.birthDate.month == youngestPerson.birthDate.month)
      {
        if (person.birthDate.day > youngestPerson.birthDate.day)
          youngestPerson = person;
      }
    }
  }

  cout << endl
       << "Информация о самом молодом человеке:\n" << youngestPerson.surname << " " << youngestPerson.name << " "
       << youngestPerson.patronymic << " " << youngestPerson.gender << " " << youngestPerson.nationality
       << " Рост: " << youngestPerson.height << " Вес: " << youngestPerson.weight
       << " Дата рождения: " << youngestPerson.birthDate.day << "/" << youngestPerson.birthDate.month << "/"
       << youngestPerson.birthDate.year << " Номер телефона: " << youngestPerson.phoneNumber
       << " Адрес: " << youngestPerson.address << endl;
}

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
    cout << "Выбранного задания нет в списке";
  }
  return 0;
}
