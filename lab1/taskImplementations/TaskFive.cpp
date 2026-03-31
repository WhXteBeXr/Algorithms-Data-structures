#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "../labFunctions.h"

using namespace std;

/* Создать текстовый файл с произвольной информацией. Организовать просмотр содержимого файла. Организовать чтение и
 * обработку данных из файла в соответствии с индивидуальным заданием. Сохранить полученные результаты в новый текстовый
 * файл. */

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
       << "Информация о самом молодом человеке:\n"
       << youngestPerson.surname << " " << youngestPerson.name << " " << youngestPerson.patronymic << " "
       << youngestPerson.gender << " " << youngestPerson.nationality << " Рост: " << youngestPerson.height
       << " Вес: " << youngestPerson.weight << " Дата рождения: " << youngestPerson.birthDate.day << "/"
       << youngestPerson.birthDate.month << "/" << youngestPerson.birthDate.year
       << " Номер телефона: " << youngestPerson.phoneNumber << " Адрес: " << youngestPerson.address << endl;
}
